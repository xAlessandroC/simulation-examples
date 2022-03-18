/*
 * UEAppINteractionTester.cc
 *
 *  Created on: Mar 17, 2022
 *      Author: simu5g
 */


// Imports
#include <sstream>

#include "UEAppInteractionTester.h"


Define_Module(UEAppInteractionTester);

UEAppInteractionTester::UEAppInteractionTester()
{

}

void UEAppInteractionTester::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    // avoid multiple initializations
    if (stage!=inet::INITSTAGE_APPLICATION_LAYER-1){
        return;
    }

    EV << "UEAppInteractionTester::initialize - stage " << stage << endl;

    localPort = par("localPort");
    targetMecAppPort = par("targetMecAppPort");

    localAddress = inet::L3AddressResolver().resolve(par("localAddress"));
    targetMecAppAddress = inet::L3AddressResolver().resolve(par("targetMecAppAddress"));

    startDelay = par("startDelay").doubleValue();
    positionX = par("positionX").doubleValue();
    positionY = par("positionY").doubleValue();
    radius = par("radius").doubleValue();


    // Set up socket for communcation
    if (localPort != -1)
    {
        socket.setOutputGate(gate("socketOut"));
        socket.bind(localPort);
    }
    EV << simTime() << "UEAppInteractionTester::initialize - binding to port: local:" << localPort << " , dest: " << targetMecAppAddress.str() << ":" << targetMecAppPort << endl;

    startProcedure_ = new cMessage("StartProcedure");
    scheduleAt(simTime() + startDelay, startProcedure_);
}

void UEAppInteractionTester::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        EV << "UEAppInteractionTester::handleMessage - self message received!" << endl;
        if (!strcmp(msg->getName(), "StartProcedure")){
            sendUeMessage();
        }
    }else{
        EV << "UEAppInteractionTester::handleMessage - other message received!" << endl;

        inet::Packet* packet = check_and_cast<inet::Packet*>(msg);
        auto mePkt = packet->peekAtFront<WarningAppPacket>();
        if(!strcmp(mePkt->getType(), WARNING_ALERT))
            handleInfoMEWarningAlertApp(msg);
    }
}

void UEAppInteractionTester::sendUeMessage(){

    delete startProcedure_;

    inet::Packet* packet = createUEPacket(positionX, positionY, radius);
    socket.sendTo(packet, targetMecAppAddress, targetMecAppPort);

    getParentModule()->bubble("UEMessage inviato...");
}

inet::Packet* UEAppInteractionTester::createUEPacket(double positionX, double positionY, double radius){

    inet::Packet* pkt = new inet::Packet("WarningAlertPacketStart");
    auto alert = inet::makeShared<WarningStartPacket>();
    alert->setType(START_WARNING);
    alert->setCenterPositionX(positionX);
    alert->setCenterPositionY(positionY);
    alert->setRadius(radius);
    alert->setChunkLength(inet::B(20));
    alert->addTagIfAbsent<inet::CreationTimeTag>()->setCreationTime(simTime());
    pkt->insertAtBack(alert);

    return pkt;
}

void UEAppInteractionTester::handleInfoMEWarningAlertApp(cMessage* msg)
{
    inet::Packet* packet = check_and_cast<inet::Packet*>(msg);
    auto pkt = packet->peekAtFront<WarningAlertPacket>();

    EV << "UEAppInteractionTester::handleInfoMEWarningrAlertApp - Received " << pkt->getType() << " type WarningAlertPacket"<< endl;

    //updating runtime color of the car icon background
    cDisplayString& dispStr = getParentModule()->getDisplayString();
    if(pkt->getDanger())
    {

        EV << "UEAppInteractionTester::handleInfoMEWarningrAlertApp - Warning Alert Detected: DANGER!" << endl;
//        dispStr.setTagArg("i",1, "yellow");
        dispStr.setTagArg("b", 0, 30);
        dispStr.setTagArg("b", 1, 30);
        dispStr.setTagArg("b", 2, "oval");
        dispStr.setTagArg("b", 3, "yellow");
        dispStr.setTagArg("b", 4, "black");
    }
    else{

        EV << "UEAppInteractionTester::handleInfoMEWarningrAlertApp - Warning Alert Detected: NO DANGER!" << endl;
//        dispStr.setTagArg("i",1, "green");
        dispStr.removeTag("b");
    }
}



