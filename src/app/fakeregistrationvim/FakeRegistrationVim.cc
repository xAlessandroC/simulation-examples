

// Imports
#include <sstream>

#include "FakeRegistrationVim.h"

Define_Module(FakeRegistrationVim);

FakeRegistrationVim::FakeRegistrationVim()
{

}

void FakeRegistrationVim::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    // avoid multiple initializations
    if (stage!=inet::INITSTAGE_APPLICATION_LAYER-1){
        return;
    }

    EV << "FakeRegistrationVim::initialize - stage " << stage << endl;

    // get reference of other modules
    vimHost = getParentModule()->getParentModule()->getSubmodule("viManager");
    if(vimHost == nullptr)
        throw cRuntimeError("FakeRegistrationVim::initialize - vimHost not found!");

    vimApp = vimHost->getSubmodule("app", 0);
    if(vimApp == nullptr)
        throw cRuntimeError("FakeRegistrationVim::initialize - vimApp not found!");
//    VirtualisationInfrastructureManagerDyn* vimApp = check_and_cast<VirtualisationInfrastructureManagerDyn*>(vimApp_module);

    cMessage* _register = new cMessage("Register");
    scheduleAt(simTime()+0.015, _register);
}

void FakeRegistrationVim::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        EV << "FakeRegistrationVim::handleMessage - self message received!" << endl;
        VirtualisationInfrastructureManagerDyn* vimAppClass = check_and_cast<VirtualisationInfrastructureManagerDyn*>(vimApp);
        int host_id = vimAppClass->registerHost(3456, 1111, 1111, 1111, inet::L3Address("192.168.1.1"), 6789);
    }else{
        EV << "FakeRegistrationVim::handleMessage - other message received!" << endl;
    }
}
