

// Imports
#include <sstream>

#include "InstantiationMecpm.h"
#include "nodes/mec/MECOrchestrator/MECOMessages/MECOrchestratorMessages_m.h"

Define_Module(InstantiationMecpm);

InstantiationMecpm::InstantiationMecpm()
{

}

void InstantiationMecpm::initialize(int stage)
{
    cSimpleModule::initialize(stage);

    // avoid multiple initializations
    if (stage!=inet::INITSTAGE_APPLICATION_LAYER-1){
        return;
    }

    EV << "InstantiationMecpm::initialize - stage " << stage << endl;

    localPort = par("localPort");
    destPort = par("destPort");

    localAddress = inet::L3AddressResolver().resolve(par("localAddress"));

    termination = par("terminate");
    termination_delay = par("term_delay");
    start_delay = par("start_delay");

    appID = par("appID").intValue();
    contextID = par("contextID").intValue();


    // Set up socket for communcation
    int port = par("localPort");
    if (port != -1)
    {
        socket.setOutputGate(gate("socketOut"));
        socket.bind(port);
    }
    EV << simTime() << "InstantiationMecpm::initialize - binding to port: local:" << localPort << " , dest: " << destAddress.str() << ":" << destPort << endl;

    initSocket_ = new cMessage("InitSocket");
    scheduleAt(simTime()+start_delay, initSocket_);
}

void InstantiationMecpm::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        EV << "InstantiationMecpm::handleMessage - self message received!" << endl;
        if (!strcmp(msg->getName(), "InitSocket")){
            initSocket();
        }
        else if (!strcmp(msg->getName(), "TerminateApp")){
            sendTermination();
        }
    }else{
        EV << "InstantiationMecpm::handleMessage - other message received!" << endl;
    }
}

void InstantiationMecpm::initSocket()
{
    EV << "InstantiationMecpm::initSocket - start" << endl;

//    std::string destAddress = par("destAddress").stringValue();
    cModule* destModule = findModuleByPath(par("destAddress").stringValue());
    destAddress = inet::L3AddressResolver().resolve(par("destAddress"));
    if (destModule == nullptr)
        throw cRuntimeError("InstantiationMecpm::initSocket - \tFATAL! Cannot find destAddress!");

//    std::cout << destModule << endl;

    delete initSocket_;

    inet::Packet* packet = createInstantiationPacket();
    socket.sendTo(packet, destAddress, destPort);

    getParentModule()->bubble("Instanziazione inviata...");


    if(termination){
        terminateApp_ = new cMessage("TerminateApp");
        scheduleAt(simTime()+termination_delay, terminateApp_);
    }
}

void InstantiationMecpm::sendTermination()
{
    EV << "InstantiationMecpm::sendTermination" << endl;

    delete terminateApp_;

    inet::Packet* packet = createTerminationPacket();
    socket.sendTo(packet, destAddress, destPort);

    getParentModule()->bubble("Terminazione inviata...");
}


inet::Packet* InstantiationMecpm::createInstantiationPacket()
{
    EV << "InstantiationMecpm::createInstantiationPacket" << endl;
//    (CreateAppMessage) sno = 0, timestamp = 0s, type = , sourceAddress = , sourcePort = 0, destinationAddress = , destinationPort = 0, destinationMecAppAddress = , destinationMecAppPort = 0, MEModuleType = simu5g.apps.mec.WarningAlert.MECWarningAlertApp, MEModuleName = MECWarningAlertApp, contextId = 0, ueAppID = 192, requiredDisk = 10000000, requiredRam = 10000000, requiredCpu = 1500, requiredService = MEWarningAlertService, providedService =

    inet::Packet* packet = new inet::Packet("Instantiation");
    auto registrationpck = inet::makeShared<CreateAppMessage>();

    registrationpck->setUeAppID(appID);
    registrationpck->setMEModuleName("MECWarningAlertApp");
    registrationpck->setMEModuleType("simu5g.apps.mec.WarningAlert.MECWarningAlertApp");

    registrationpck->setRequiredCpu(1000);
    registrationpck->setRequiredRam(1000);
    registrationpck->setRequiredDisk(1000);

    registrationpck->setRequiredService("MEWarningAlertService");
    registrationpck->setContextId(contextID);

    registrationpck->setChunkLength(inet::B(2000));
    packet->insertAtBack(registrationpck);

    return packet;
}

inet::Packet* InstantiationMecpm::createTerminationPacket()
{
    EV << "InstantiationMecpm::createTerminationPacket" << endl;

    inet::Packet* packet = new inet::Packet("Termination");
    auto terminationpck = inet::makeShared<DeleteAppMessage>();

    terminationpck->setUeAppID(appID);
    terminationpck->setChunkLength(inet::B(100));
    packet->insertAtBack(terminationpck);

    return packet;
}



