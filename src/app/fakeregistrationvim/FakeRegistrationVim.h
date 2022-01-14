

#ifndef APP_FAKEREGISTRATION_H_
#define APP_FAKEREGISTRATION_H_

//BINDER and UTILITIES
#include "common/LteCommon.h"
#include "common/binder/Binder.h"
#include "inet/networklayer/common/InterfaceTable.h"

//INET
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/common/L3AddressResolver.h"

#include "nodes/mec/utils/MecCommon.h"

#include "nodes/mec/VirtualisationInfrastructureManager/Dynamic/VirtualisationInfrastructureManagerDyn.h"

#define NO_SERVICE -1
#define SERVICE_NOT_AVAILABLE -2

using namespace omnetpp;

class FakeRegistrationVim : public cSimpleModule
{
    // Reference to other modules
    cModule* vimHost;
    cModule* vimApp;
//    VirtualisationInfrastructureManagerDyn* vimApp;

    public:
        FakeRegistrationVim();

    protected:

        virtual int numInitStages() const { return inet::NUM_INIT_STAGES; }

        void initialize(int stage);

        virtual void handleMessage(cMessage *msg);
};


#endif
