/*
 * NodeA.h
 *
 *  Created on: Dec 21, 2021
 *      Author: simu5g
 */

#ifndef NODE_B_H_
#define NODE_B_H_

#include <string>
#include <omnetpp.h>

using namespace omnetpp;

class NodeB : public cSimpleModule
{
    int code;
    float value;

    std::string name;

    public:
        NodeB();

        float getValue(){
            return 2.2f;
        }

    protected:
        virtual void initialize(int stage);
        virtual void handleMessage(cMessage *msg);

        std::string getNodeName();
        int getCode();

};

#endif
