
#ifndef APP_REGISTRATION_H
#define APP_REGISTRATION_H

//BINDER and UTILITIES
#include "common/LteCommon.h"
#include "common/binder/Binder.h"
#include "inet/networklayer/common/InterfaceTable.h"

//INET
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/common/L3AddressResolver.h"

#include "nodes/mec/utils/MecCommon.h"

#include <inet/transportlayer/contract/udp/UdpSocket.h>
#include <omnetpp.h>

#include "msg/RegistrationPacket_m.h"

#include "nodes/mec/VirtualisationInfrastructureManager/Dynamic/VirtualisationInfrastructureManagerDyn.h"

#define NO_SERVICE -1
#define SERVICE_NOT_AVAILABLE -2

using namespace omnetpp;

class RegistrationVim : public cSimpleModule
{

    inet::UdpSocket socket;
    int localPort;
    int destPort;
    inet::L3Address destAddress;
    inet::L3Address localAddress;

    //Messages
    omnetpp::cMessage *initSocket_;

    private:
        void initSocket();

    public:
        RegistrationVim();

    protected:

        virtual int numInitStages() const { return inet::NUM_INIT_STAGES; }

        void initialize(int stage);

        virtual void handleMessage(cMessage *msg);

    private:

        inet::Packet* createRegistrationPacket();

        inet::Packet* createInstantiationPacket();

};


#endif
