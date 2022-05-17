//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "MobilityTester.h"

Define_Module(MobilityTester);

void MobilityTester::initialize(int stage)
{
    if(stage != inet::INITSTAGE_APPLICATION_LAYER)
      return;


    // Socket local binding
    tcpSocket.setOutputGate(gate("socketOut"));
    tcpSocket.bind(localPort);
    tcpSocket.setCallback(this);

    const char *lcAddress = par("localAddress").stringValue();
    localIPAddress = inet::L3AddressResolver().resolve(lcAddress);

    destPort = par("remotePort");
    destIPAddress = inet::L3AddressResolver().resolve(par("remoteAddress").stringValue());

    cMessage *msg = new cMessage("connect");
    scheduleAt(simTime()+0.1, msg);
}

void MobilityTester::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage() && strcmp(msg->getFullName(), "connect") == 0)
    {
        tcpSocket.renewSocket();
        EV << "MobilityTester::Received connect" << endl;
        tcpSocket.connect(destIPAddress, destPort);
        delete msg;
        cMessage *sendRequest = new cMessage("request");
        scheduleAt(simTime()+0.03, sendRequest);
    }
    else if(msg->isSelfMessage() && strcmp(msg->getFullName(), "request") == 0)
    {
        EV << "MobilityTester::Sending HTTP request - POST" << endl;
        sendPostRequest();
    }
    else if(msg->isSelfMessage() && strcmp(msg->getFullName(), "getrequest") == 0)
    {
        EV << "MobilityTester::Sending HTTP request - GET" << endl;
        sendGetRequest();
    }
    else
    {
        if(msg->arrivedOn("socketIn"))
        {
            ASSERT(tcpSocket && tcpSocket.belongsToSocket(msg));
            tcpSocket.processMessage(msg);
        }

    }
}

void MobilityTester::sendPostRequest()
{
    std::string uri("/example/amsi/v1/app_mobility_services/");
    std::string serverHost = tcpSocket.getRemoteAddress().str() + ":" + std::to_string(tcpSocket.getRemotePort());

    nlohmann::ordered_json request;

    request["appMobilityServiceId"] = "";
    request["deviceInformation"] = nlohmann::json::array();
    nlohmann::ordered_json val;
    val["associateId"]["type"] = "UE_IPV4_ADDRESS";
    val["associateId"]["value"] = localIPAddress.str();
    val["appMobilityServiceLevel"] = "APP_MOBILITY_NOT_ALLOWED";
    val["contextTransferState"] = "NOT_TRANSFERRED";
    request["deviceInformation"].push_back(val);
    nlohmann::ordered_json val2;
    val2["associateId"]["type"] = "UE_IPV4_ADDRESS";
    val2["associateId"]["value"] = "10.0.0.3";
    val2["appMobilityServiceLevel"] = "APP_MOBILITY_WITH_CONFIRMATION";
    val2["contextTransferState"] = "NOT_TRANSFERRED";
    request["deviceInformation"].push_back(val2);
    request["expiryTime"] = 0;
    request["serviceConsumerId"]["appInstanceId"] = std::to_string(getId());
    request["serviceConsumerId"]["mepId"] = std::to_string(getId()+1); // mec platform id

    Http::sendPostRequest(&tcpSocket, request.dump().c_str(), serverHost.c_str(), uri.c_str());
//    cMessage *sendRequest = new cMessage("getrequest");
//    scheduleAt(simTime()+0.03, sendRequest);
}

void MobilityTester::sendGetRequest()
{
    EV << "MobilityTester::sending GET Request" << endl;

    EV << "MobilityTester::sending total" << endl;
    std::string uri("/example/amsi/v1/app_mobility_services/");
    std::string serverHost = tcpSocket.getRemoteAddress().str() + ":" + std::to_string(tcpSocket.getRemotePort());

    Http::sendGetRequest(&tcpSocket, serverHost.c_str(), uri.c_str());

    EV << "MobilityTester::sending GET id" << endl;
//    std::string uri2("/example/amsi/v1/app_mobility_services/0");
//    Http::sendGetRequest(&tcpSocket, serverHost.c_str(), uri2.c_str());

//    std::string uri3("/example/amsi/v1/app_mobility_services/0/deregister_task");
//    Http::sendGetRequest(&tcpSocket, serverHost.c_str(), uri3.c_str());

    //std::string uri2("/example/amsi/v1/app_mobility_services/0");
    //Http::sendDeleteRequest(&tcpSocket, serverHost.c_str(), uri2.c_str());
    std::string uri2("/example/amsi/v1/app_mobility_services/0");
    nlohmann::ordered_json request;
    request["deviceInformation"] = nlohmann::json::array();
    nlohmann::ordered_json val;
    val["associateId"]["type"] = "UE_IPV4_ADDRESS";
    val["associateId"]["value"] = localIPAddress.str();
    val["appMobilityServiceLevel"] = "APP_MOBILITY_WITH_CONFIRMATION";
    val["contextTransferState"] = "NOT_TRANSFERRED";
    request["deviceInformation"].push_back(val);
    request["expiryTime"] = 0;
    request["serviceConsumerId"]["appInstanceId"] = std::to_string(getId());
    request["serviceConsumerId"]["mepId"] = std::to_string(getId()+1); // mec platform id

    Http::sendPutRequest(&tcpSocket, request.dump().c_str(), serverHost.c_str(), uri2.c_str());
}
void MobilityTester::socketEstablished(inet::TcpSocket *socket)
{
    EV << "MobilityTester::Connection Enstablished" << endl;
}

void MobilityTester::socketDataArrived(inet::TcpSocket *socket, inet::Packet *packet, bool urgent)
{
    EV << "MobilityTester::Data Arrived" << endl;
    std::vector<uint8_t> bytes =  packet->peekDataAsBytes()->getBytes();
    delete packet;

    std::string msg(bytes.begin(), bytes.end());
    bool res = Http::parseReceivedMsg(msg, &buffer, &currentHttpMessage);
    if(res)
    {
        if(currentHttpMessage->getType() == RESPONSE)
        {
            HttpResponseMessage * response = dynamic_cast<HttpResponseMessage*> (currentHttpMessage);
            EV << "MobilityTester::Correct HTTPResponse" << endl;
            nlohmann::json jsonResponseBody = nlohmann::json::parse(response->getBody());
            EV << "MobilityTester::code: " << response->getCode() << endl;
            EV << "MobilityTester::json response" << jsonResponseBody.dump().c_str() << endl;
        }
    }

    if(currentHttpMessage != nullptr)
    {
        currentHttpMessage = nullptr;
    }
}
