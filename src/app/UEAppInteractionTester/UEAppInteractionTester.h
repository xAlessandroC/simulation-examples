/*
 * UEAPPInteractionTester.h
 *
 *  Created on: Mar 17, 2022
 *      Author: simu5g
 */

#ifndef APP_UEAPPINTERACTIONTESTER_UEAPPINTERACTIONTESTER_H_
#define APP_UEAPPINTERACTIONTESTER_UEAPPINTERACTIONTESTER_H_

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

#include "apps/mec/WarningAlert/packets/WarningAlertPacket_m.h"
#include "apps/mec/WarningAlert/packets/WarningAlertPacket_Types.h"
#include "inet/common/TimeTag_m.h"

using namespace omnetpp;

class UEAppInteractionTester : public cSimpleModule
{

    inet::UdpSocket socket;

    int localPort;
    inet::L3Address localAddress;

    int targetMecAppPort;
    inet::L3Address targetMecAppAddress;

    double startDelay;
    double positionX;
    double positionY;
    double radius;

    // Messages
    cMessage* startProcedure_;

    public:
        UEAppInteractionTester();

    protected:

        virtual int numInitStages() const { return inet::NUM_INIT_STAGES; }

        void initialize(int stage);

        virtual void handleMessage(cMessage *msg);

        void sendUeMessage();

    private:

        inet::Packet* createUEPacket(double positionX, double positionY, double radius);

        void handleInfoMEWarningAlertApp(cMessage* msg);
};



#endif /* APP_UEAPPINTERACTIONTESTER_UEAPPINTERACTIONTESTER_H_ */
