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

#include "TriggerAMS.h"

Define_Module(TriggerAMS);

void TriggerAMS::initialize(int stage)
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

void TriggerAMS::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage() && strcmp(msg->getFullName(), "connect") == 0)
    {
        tcpSocket.renewSocket();
        EV << "MobilityTester::Received connect" << endl;
        tcpSocket.connect(destIPAddress, destPort);
        delete msg;
        cMessage *trigger = new cMessage("trigger");
        scheduleAt(simTime()+1, trigger);
    }
    else if(msg->isSelfMessage() && strcmp(msg->getFullName(), "trigger") == 0)
    {
        std::string uri("/example/amsi/v1/eventNotification/");
        std::string serverHost = tcpSocket.getRemoteAddress().str() + ":" + std::to_string(tcpSocket.getRemotePort());

        nlohmann::ordered_json request;
        request["notificationType"] = "MobilityProcedureNotification";
        request["associateId"] = nlohmann::json::array();

        request["mobilityStatus"] = "INTERHOST_MOVEOUT_TRIGGERED";
        request["_links"]["href"] = "";

    //    if(targetAppInfo == nullptr)
    //    {
    //        object["targetAppInfo"]["appInstanceId"] = "";
    //        object["targetAppInfo"]["commInterface"]["ipAddresses"] = nlohmann::json::array();
    //        nlohmann::ordered_json empty_obj;
    //        empty_obj["host"] = "";
    //        empty_obj["port"] = 0;
    //        object["targetAppInfo"]["commInterface"]["ipAddresses"].push_back(empty_obj);
    //    }
        //object["targetAppInfo"] = nlohmann::json::;

        request["appInstanceId"] = "MECWarningAlertApp[0]";

        EV << "Trigger ready: " << request.dump() << endl;

        Http::sendPostRequest(&tcpSocket, request.dump().c_str(), serverHost.c_str(), uri.c_str());
    }
}

void TriggerAMS::socketEstablished(inet::TcpSocket *socket)
{
    EV << "Trigger::Socket established"<<endl;
    cMessage *msg = new cMessage("trigger");
    scheduleAt(simTime()+0.1, msg);
}
