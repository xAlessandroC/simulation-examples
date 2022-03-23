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

#include "MEOTester.h"
#include "nodes/mec/MECOrchestrator/MECOMessages/MECOrchestratorMessages_m.h" // TODO add this messages to our message

Define_Module(MEOTester);

void MEOTester::initialize(int stage)
{
    if(stage != inet::INITSTAGE_APPLICATION_LAYER)
        return;

    EV << "MEOTester::initialization" << endl;

    const char *referencePoint = par("referencePointType");

    // This is done just to test the MEO behaviour
    // this must not be done in real simulated scenarios
    if(std::strcmp(referencePoint, "mm4") == 0)
        mm = MM4;
    else
        mm = MM3;

    localPort = par("localPort");

    udpSocket.setOutputGate(gate("socketOut"));
    localAddress = par("localAddress");
    udpSocket.bind(*localAddress ? inet::L3AddressResolver().resolve(localAddress) : inet::L3Address(), par("localPort"));

    meoPort = par("meoPort");
    const char *meoAddress = par("meoAddress");
    meoIPAddress = inet::L3AddressResolver().resolve(meoAddress);

    cMessage *msg = new cMessage("start");
    scheduleAt(simTime()+0.05, msg);
}

void MEOTester::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage() && std::strcmp(msg->getName(), "start") == 0)
    {
        EV << "MEOTester::Received start!" << endl;
        udpSocket.connect(meoIPAddress, meoPort);
//        cMessage *toSend = new cMessage("send");
//        scheduleAt(simTime()+0.03, toSend);
        sendMessage();
    }else if(msg->isSelfMessage() && std::strcmp(msg->getName(), "send") == 0)
    {
        EV << "MEOTester::Sending Message" << endl;
        sendMessage();
    }
    else
    {
        inet::Packet* packet = check_and_cast<inet::Packet *>(msg);
        EV << "MEOTester::Received another message " << msg->getName() << endl;
        if(mm == MM3 && std::strcmp(packet->getName(), "ServiceRequest") == 0)
        {
            // MECPlatformManager
            auto data = packet->peekData<MeoMepmRequest>();
            EV << "MEOTester::MEPM data: " << endl;
            const MeoMepmRequest* receivedData = data.get();
            for(int i = 0; i < receivedData->getRequiredServiceNamesArraySize(); i++)
            {
                EV << "MEOTester::RequiredServiceName: " << receivedData->getRequiredServiceNames(i);
            }

            // Send back a response with yes!
            inet::Packet* packet = new inet::Packet("AvailabilityResponseMepm");
            auto responsePkt = inet::makeShared<MecHostResponse>();
            responsePkt->setDeviceAppId(receivedData->getDeviceAppId());
            responsePkt->setMecHostId(getParentModule()->getId());
            responsePkt->setResult(true);
            responsePkt->setChunkLength(inet::B(2000));
            packet->insertAtBack(responsePkt);
            udpSocket.send(packet);


        }
        else if(mm == MM3 && std::strcmp(packet->getName(), "instantiationApplicationRequest") == 0)
        {
            // Create an appInfo packet to send back
            // I have created the infrastructure Instantiation response that should be used
            EV << "MEOTester::MEPM received instantiation Application Request from orchestrator " << endl;
            auto data = packet->peekData<CreateAppMessage>();
            const CreateAppMessage *createMsg = data.get();

            EV << "MEPM::UEAppId: " << createMsg->getUeAppID() << endl;
            EV << "MEPM::MEModulename: " << createMsg->getMEModuleName() << endl;
            EV << "MEPM::MEModuleType: " << createMsg->getMEModuleType() << endl;
            EV << "MEPM::Required cpu: " << createMsg->getRequiredCpu() << ", ram: " << createMsg->getRequiredRam() << ", disk: " << createMsg->getRequiredDisk() << endl;

            // Replying back
            inet::Packet* toSend = new inet::Packet("instantiationApplicationResponse");
            auto responsePkt = inet::makeShared<InstantiationApplicationResponse>();
            responsePkt->setStatus(true);
            responsePkt->setMecHostId(getParentModule()->getId());
            responsePkt->setAppName(createMsg->getMEModuleName()); // send back module name to avoid findingLoop
            responsePkt->setDeviceAppId(std::to_string(createMsg->getUeAppID()).c_str());
            std::stringstream appName;
            appName << createMsg->getMEModuleName() << "[" <<  createMsg->getContextId() << "]";
            responsePkt->setInstanceId(appName.str().c_str());
            inet::L3Address localIpAddress = inet::L3AddressResolver().resolve(localAddress);
            responsePkt->setMecAppRemoteAddress(localIpAddress);
            responsePkt->setMecAppRemotePort(3456); // to change
            EV << "Context id: " << createMsg->getContextId() << endl;
            responsePkt->setContextId(createMsg->getContextId());
            responsePkt->setChunkLength(inet::B(1000));
            toSend->insertAtBack(responsePkt);
            udpSocket.send(toSend);




        }
        else
        {
            // VirtualisationInfrastructureManager
            auto data = packet->peekData<MeoVimRequest>();
            const MeoVimRequest *receivedData = data.get();

            EV << "MEOTester::VIM Data: cpu: " << receivedData->getCpu() << ", disk: " << receivedData->getDisk() << ", ram: " << receivedData->getRam() << endl;
            // Send back a response with yes!
            inet::Packet* packet = new inet::Packet("AvailabilityResponseVim");
            auto responsePkt = inet::makeShared<MecHostResponse>();
            responsePkt->setDeviceAppId(receivedData->getDeviceAppId());
            responsePkt->setMecHostId(this->getParentModule()->getId());
            responsePkt->setResult(true);
            responsePkt->setChunkLength(inet::B(2000));
            packet->insertAtBack(responsePkt);
            udpSocket.send(packet);
        }
    }

    delete msg;

}


void MEOTester::sendMessage()
{
    inet::Packet* packet = new inet::Packet("Registration");
    auto registrationPkt = inet::makeShared<RegistrationPkt>();
    EV << "mm: " << mm << endl;
    registrationPkt->setHostId(getParentModule()->getId());
    inet::L3Address myAddress = inet::L3AddressResolver().resolve(localAddress);
//    registrationPkt->setSourceAddress(myAddress);
    registrationPkt->setType(mm);
    registrationPkt->setSourcePort(localPort);
    registrationPkt->setChunkLength(inet::B(2000));
    packet->insertAtBack(registrationPkt);

    EV << "MEOTester::sending custom packet to MEO " << endl;
    udpSocket.send(packet);

}
