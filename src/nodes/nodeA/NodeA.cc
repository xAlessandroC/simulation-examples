/*
 * NodeA.cc
 *
 *  Created on: Dec 21, 2021
 *      Author: simu5g
 */

#include "NodeA.h";

Define_Module(NodeA);

NodeA::NodeA()
{
    name = "NODE A";
    code = 1;
}

void NodeA::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    // avoid multiple initializations
//    if (stage != 11)
//        return;

    EV << "NodeA " << getNodeName() << "::initialize - stage " << stage << endl;
    EV << "NodeA: float value " << getValue() << endl;

    if(strcmp("node1", getName()) == 0)
    {
        EV << "NodeA " << getNodeName() << "::sending message" << endl;
        cMessage *msg = new cMessage("FakeMessage");
        send(msg, "nodeAOut");
    }
}

void NodeA::handleMessage(cMessage *msg)
{
    send(msg, "nodeAOut");
}

std::string NodeA::getNodeName()
{
    return this -> name;
}

int NodeA::getCode()
{
    return this -> code;
}



