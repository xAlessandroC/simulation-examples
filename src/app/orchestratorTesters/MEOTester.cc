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

    delete msg;

}


void MEOTester::sendMessage()
{
    inet::Packet* packet = new inet::Packet("Registration");
    auto registrationPkt = inet::makeShared<RegistrationPkt>();
    EV << "mm: " << mm << endl;
    registrationPkt->setHostId(getParentModule()->getId());
    inet::L3Address myAddress = inet::L3AddressResolver().resolve(localAddress);
    registrationPkt->setSourceAddress(myAddress);
    registrationPkt->setType(mm);
    registrationPkt->setSourcePort(localPort);
    registrationPkt->setChunkLength(inet::B(2000));
    packet->insertAtBack(registrationPkt);

    EV << "MEOTester::sending custom packet to MEO " << endl;
    udpSocket.send(packet);

}
