/*
 * NodeA.cc
 *
 *  Created on: Dec 21, 2021
 *      Author: simu5g
 */

#include "NodeB.h"

Define_Module(NodeB);

NodeB::NodeB()
{
    name = "NODE B";
    code = 1;
}

void NodeB::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    // avoid multiple initializations
//    if (stage != 11)
//        return;

    EV << "NodeB " << getNodeName() << "::initialize - stage " << stage << endl;

    if(strcmp("node1", getName()) == 0)
    {
        EV << "NodeB " << getNodeName() << "::sending message" << endl;
        cMessage *msg = new cMessage("FakeMessage");
        send(msg, "nodeAOut");
    }
}

void NodeB::handleMessage(cMessage *msg)
{
    send(msg, "nodeAOut");
}

std::string NodeB::getNodeName()
{
    return this -> name;
}

int NodeB::getCode()
{
    return this -> code;
}



