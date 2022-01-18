

// Imports
#include <sstream>

#include "RegistrationVim.h"
#include "nodes/mec/MECOrchestrator/MECOMessages/MECOrchestratorMessages_m.h"

Define_Module(RegistrationVim);

RegistrationVim::RegistrationVim()
{

}

void RegistrationVim::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    // avoid multiple initializations
    if (stage!=inet::INITSTAGE_APPLICATION_LAYER-1){
        return;
    }

    EV << "RegistrationVim::initialize - stage " << stage << endl;

    localPort = par("localPort");
    destPort = par("destPort");

    localAddress = inet::L3AddressResolver().resolve(par("localAddress"));

    // Set up socket for communcation
    int port = par("localPort");
    if (port != -1)
    {
        socket.setOutputGate(gate("socketOut"));
        socket.bind(port);
    }
    EV << simTime() << "RegistrationVim::initialize - binding to port: local:" << localPort << " , dest: " << destAddress.str() << ":" << destPort << endl;

    initSocket_ = new cMessage("InitSocket");
    scheduleAt(simTime()+1.0, initSocket_);
}

void RegistrationVim::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        EV << "RegistrationVim::handleMessage - self message received!" << endl;
        if (!strcmp(msg->getName(), "InitSocket")){
            initSocket();
        }
    }else{
        EV << "RegistrationVim::handleMessage - other message received!" << endl;
    }
}

void RegistrationVim::initSocket()
{
    EV << "RegistrationVim::initSocket - start" << endl;

//    std::string destAddress = par("destAddress").stringValue();
    cModule* destModule = findModuleByPath(par("destAddress").stringValue());
    destAddress = inet::L3AddressResolver().resolve(par("destAddress"));
    if (destModule == nullptr)
        throw cRuntimeError("RegistrationVim::initSocket - \tFATAL! Cannot find destAddress!");

    std::cout << destModule << endl;

    delete initSocket_;

//    inet::Packet* packet = createRegistrationPacket();
    inet::Packet* packet = createInstantiationPacket();
    socket.sendTo(packet, destAddress, destPort);

    getParentModule()->bubble("Registrazione inviata...");
}

inet::Packet* RegistrationVim::createRegistrationPacket()
{
    EV << "RegistrationVim::createRegistrationPacket" << endl;

    inet::Packet* packet = new inet::Packet("Register");
    auto registrationpck = inet::makeShared<RegistrationPacket>();
    registrationpck->setRam(7777);
    registrationpck->setDisk(7777);
    registrationpck->setCpu(7777);
    registrationpck->setAddress(localAddress);
    registrationpck->setChunkLength(inet::B(200));
    packet->insertAtBack(registrationpck);

    return packet;
}

inet::Packet* RegistrationVim::createInstantiationPacket()
{
    EV << "RegistrationVim::createInstantiationPacket" << endl;

    inet::Packet* packet = new inet::Packet("Instantiation");
    auto registrationpck = inet::makeShared<CreateAppMessage>();

    registrationpck->setUeAppID(233);
    registrationpck->setMEModuleName("MEModuleName");
    registrationpck->setMEModuleType("MEModuleType");

    registrationpck->setRequiredCpu(1000);
    registrationpck->setRequiredRam(1000);
    registrationpck->setRequiredDisk(1000);

    registrationpck->setRequiredService("NULL");
    registrationpck->setContextId(22);

    registrationpck->setChunkLength(inet::B(2000));
    packet->insertAtBack(registrationpck);

    return packet;
}



