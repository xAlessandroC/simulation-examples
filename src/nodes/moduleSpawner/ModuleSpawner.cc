
#include "ModuleSpawner.h"
#include "inet/networklayer/common/InterfaceTable.h"

Define_Module(ModuleSpawner);

ModuleSpawner::ModuleSpawner(){

}

ModuleSpawner::~ModuleSpawner(){

    cancelAndDelete(createNewPC_);
    cancelAndDelete(createNewUE_);
    cancelAndDelete(burstGeneration_);
    cancelAndDelete(createNewDebugPC_);
    cancelAndDelete(createNewDebugUE_);
    cancelAndDelete(initializePark_);
}

void ModuleSpawner::initialize(int stage)
{
    EV << "ModuleSpawner::initialize stage " << stage << endl;

    parkedCars_ = registerSignal("parkedCars");
    ues_ = registerSignal("ues");

    createNewPC_ = new cMessage("createNewPC");
    createNewUE_ = new cMessage("createNewUE");
    createNewDebugPC_ = new cMessage("createNewDebugPC");
    createNewDebugUE_ = new cMessage("createNewDebugUE");
    burstGeneration_ = new cMessage("generateBurst");
    initializePark_ = new cMessage("initializePark");

    PCcounter = 0;
    PCcreatedcounter = 0;
    UEcounter = 0;
    UEcreatedcounter = 0;

    timeline = par("initTimeLine").intValue();

    vimApp = getParentModule()->getSubmodule("mechost1")->getSubmodule("vim")->getSubmodule("vimApp");

    intervalStart = par("intervalStart").doubleValue();
    intervalBurst = par("intervalBurst").doubleValue();
    parkcapacity_ = par("parkCapacity").intValue();
    initialParkedCars = par("initialParkedCars").intValue();
    initialUes = par("initialUes").intValue();
    delta = par("delta").doubleValue();

    initFromFile = par("initFromFile").boolValue();
    filename_ << "spawnerInitData-" << parkcapacity_ << ".txt";
    if(initFromFile){
        initializeFromFile();
    }

    if(initialParkedCars == 0){
        includeMecHost(true);
    }

    getParentModule()->subscribe("logicTerminated", this);
    getParentModule()->subscribe("parkingReleased", this);


//    scheduleAfter(1.0, createNewDebugPC_);
//    scheduleAfter(2.0, createNewDebugUE_);

//    scheduleAfter(4.0, new cMessage("createNewDebugPC_"));
//    scheduleAfter(4.0, new cMessage("createNewDebugUE_"));

    scheduleAt(intervalStart+5, burstGeneration_);
    scheduleAt(intervalStart+0.1, initializePark_);
}

void ModuleSpawner::initializeFromFile(){
    std::ifstream inputFile;
    inputFile.open(filename_.str().c_str(), std::ios::in);

    std::string line;
    std::getline(inputFile, line);
    initialParkedCars = std::stoi(line);
    EV << "number of parked car " << initialParkedCars << endl;
    std::cout << "number of parked car " << initialParkedCars << endl;

    std::getline(inputFile, line);
    size_t pos = 0;
    std::string token;
    std::cout << "Times: ";
    while((pos = line.find(",")) != std::string::npos){
        token = line.substr(0, pos);
        line.erase(0, pos + 1);
        initParkTimes.push_back(stod(token));
        EV << "time " << token << endl;
        std::cout << "time " << token;
    }
    std::cout << " " << endl;

    inputFile.close();
}

void ModuleSpawner::handleMessage(cMessage *msg)
{
    EV << "ModuleSpawner::new message arrived" << endl;
    if(strcmp(msg->getName(), "initializePark") == 0)
    {
        initializePark();
        initializeUes();
    }
    else if(strcmp(msg->getName(), "createNewPC") == 0){
        EV << "Generating PC at " << simTime() << endl;
        generationTimesPCs.pop();
        generateNewPC();

        if(generationTimesPCs.size() > 0){
            double nextGenerationTime = generationTimesPCs.front();
            scheduleAt(nextGenerationTime, createNewPC_);
        }
    }
    else if(strcmp(msg->getName(), "createNewUE") == 0){
        EV << "Generating UE at " << simTime() << endl;
        generationTimesUEs.pop();
        generateNewUE();

        if(generationTimesUEs.size() > 0){
            double nextGenerationTime = generationTimesUEs.front();
            scheduleAt(nextGenerationTime, createNewUE_);
        }
    }
    else if(strcmp(msg->getName(), "generateBurst") == 0){

        intervalStart = simTime().dbl();

        // Possibile problema: generazione di due PC o due UE nello stesso esatto instante (stesso double), molto improbabile
        // Possibile problema: UE generate prima di una PC
        // TODO inserire dimensione burst basato su distribuzioni
        double lambda_cars = lambdaValuesVehicles[timeline] * parkcapacity_;
        int burst_cars_dim = static_cast<int>(poisson(lambda_cars)); // poisson distribution cars
        int burst_ue_dim = static_cast<int>(poisson(lambdaValuesUEs[timeline])); // poisson distribution ues
        burst_ue_dim = burst_ue_dim;

        std::cout << "Bursting cars: " << burst_cars_dim << ", Bursting UEs: " << burst_ue_dim << ", TIME: " << timeline << endl;

        if((burst_cars_dim + PCcounter) > parkcapacity_)
            burst_cars_dim = parkcapacity_ - PCcounter;

        emit(parkedCars_, PCcounter);
        emit(ues_, UEcounter);

        generatePCBurst(burst_cars_dim);
        generateUEBurst(burst_ue_dim);

        timeline += 1;
        scheduleAfter(intervalBurst, burstGeneration_);
    }
    else if(strcmp(msg->getName(), "createNewDebugPC") == 0){
        generateNewPC();
    }
    else if(strcmp(msg->getName(), "createNewDebugUE") == 0){
        generateNewUE();
    }
    else{
        throw cRuntimeError("ModuleSpawner::handleMessageWhenUp - message not recognized");
    }
}


void ModuleSpawner::initializePark() {

    int parkTime;
    for(int i = 0; i < initialParkedCars; i++)
    {
        parkTime = -1;
        if(!initParkTimes.empty())
            parkTime = initParkTimes[i];
        generateNewPC(parkTime);
    }
}


void ModuleSpawner::initializeUes()
{
    for(int i = 0; i < initialUes; i++)
    {
        generateNewUE();
    }
}



void ModuleSpawner::generatePCBurst(int burstDim){
    std::vector<double> generationTimes;

    for(int i=0; i<burstDim; i++){
        generationTimes.push_back(uniform(intervalStart, intervalStart + intervalBurst - delta));
    }

    std::sort(generationTimes.begin(), generationTimes.end());
    EV << "PC generation times " << endl;
    for(double genTime: generationTimes){
        EV << genTime << "--> ";
        generationTimesPCs.push(genTime);
    }

    if(generationTimesPCs.size() > 0 && !createNewPC_->isScheduled()){
        double firstGenerationTime = generationTimesPCs.front();
        EV << "Starting scheduling for PCs... " << firstGenerationTime << endl;
        scheduleAt(firstGenerationTime, createNewPC_);
    }
}

void ModuleSpawner::generateUEBurst(int burstDim){
    std::vector<double> generationTimes;

    for(int i=0; i<burstDim; i++){
        generationTimes.push_back(uniform(intervalStart, intervalStart + intervalBurst - delta));
    }

    std::sort(generationTimes.begin(), generationTimes.end());
    EV << "UE generation times " << endl;
    for(double genTime: generationTimes){
        EV << genTime << "--> ";
        generationTimesUEs.push(genTime);
    }

    if(generationTimesUEs.size() > 0 && !createNewUE_->isScheduled()){
        double firstGenerationTime = generationTimesUEs.front();
        EV << "Starting scheduling for UEs... " << firstGenerationTime << endl;
        scheduleAt(firstGenerationTime, createNewUE_);
    }
}

void ModuleSpawner::generateNewPC(double initParkTime){
    EV << "ModuleSpawner::generating new PC" << endl;


    PCcounter++;
    PCcreatedcounter++;
    std::stringstream appName;
    appName << "PC" << PCcreatedcounter;
    std::stringstream display;
    display << "p=50,302;i=block/control;is=s";

    // initialization Module
    auto PCModuleName = "SpawnedMecCar";
    cModuleType* PCModuleType = cModuleType::get("simu5g.nodes.cars.SpawnedMecCar");
    cModule* PCmodule = PCModuleType->create(PCModuleName, getParentModule());
    PCmodule->setName(appName.str().c_str());
    PCmodule->setDisplayString(display.str().c_str());
    PCs.push_back(PCmodule);

    PCmodule->finalizeParameters();

    PCmodule->buildInside();

    PCmodule->getSubmodule("viApp")->par("localAddress") = appName.str().c_str();
    PCmodule->getSubmodule("app", 0)->par("localAddress") = appName.str().c_str();
    // TODO inserire parkTime secondo distribuzione
    // mean = 202.80
    // stddev = 135.07
    double parkTime = truncnormal(202.80, 135.07) * 60; // from minutes to seconds
    if(initParkTime != -1)
    {
        parkTime = initParkTime;
    }
    std::cout << "Time car park: " << parkTime << ", actualTime: " << simTime() << endl;
    PCmodule->getSubmodule("app", 0)->par("parkTime") = parkTime;
    startParkTimes.push_back(simTime().dbl());

//    PCmodule->getSubmodule("mobility")->par("initialX") = 50.0;
//    PCmodule->getSubmodule("mobility")->par("initialY") = 302.0 + (PCcreatedcounter - 1) * 70;

    PCmodule->scheduleStart(simTime());
    PCmodule->callInitialize();

    if(PCcounter == 1){
        includeMecHost(false);
    }

    EV << "ModuleSpawner::new PC started" << endl;
}

void ModuleSpawner::generateNewUE(){
    EV << "ModuleSpawner::generating new UE" << endl;

    UEcounter++;
    UEcreatedcounter++;
    std::stringstream appName;
    appName << "UE" << UEcreatedcounter;

    // initialization Module
    auto UEModuleName = "SpawnedNRUe";
    cModuleType* UEModuleType = cModuleType::get("simu5g.nodes.cars.SpawnedNRUe");
    cModule* UEmodule = UEModuleType->create(UEModuleName, getParentModule());
    UEmodule->setName(appName.str().c_str());
    UEs.push_back(UEmodule);

    UEmodule->finalizeParameters();
    UEmodule->buildInside();

    UEmodule->getSubmodule("app", 1)->par("deviceAppAddress") = appName.str().c_str();
    UEmodule->getSubmodule("mobility")->par("initialX") = 250.0;
    UEmodule->getSubmodule("mobility")->par("initialY") = 411.0;

    UEmodule->scheduleStart(simTime());
    UEmodule->callInitialize();

    EV << "ModuleSpawner::new UE started" << endl;
}

void ModuleSpawner::deletePC(const char* PCName){
    Enter_Method_Silent();

    for(auto it=PCs.begin(); it<PCs.end(); it++){
        cModule* selectedModule = *it;
        if(std::strcmp(selectedModule->getName(),PCName) == 0){
            selectedModule->callFinish();
            selectedModule->deleteModule();

            PCs.erase(it);
            break;
        }
    }
    PCcounter--;

    if(PCcounter == 0){
        includeMecHost(true);
    }
}

void ModuleSpawner::deleteUE(const char*  UEName){
    for(auto it=UEs.begin(); it<UEs.end(); it++){
        cModule* selectedModule = *it;
        if(std::strcmp(selectedModule->getName(),UEName) == 0){
            selectedModule->callFinish();
            selectedModule->deleteModule();
            UEs.erase(it);
            break;
        }
    }
    UEcounter--;
}

void ModuleSpawner::includeMecHost(bool include){

    vimApp->par("skipLocalResources") = !include;
}

// Listener methods
void ModuleSpawner::receiveSignal(cComponent *source, simsignal_t signalID, const char *s, cObject *details){
    EV << "ModuleSpawner::received signal " << signalID << " from " << source << " " << s << " " << details << endl;
    const char* signal_name = getSignalName(signalID);

    if(std::strcmp(signal_name, "parkingReleased") == 0){
        deletePC(s);
    }
    else if(std::strcmp(signal_name, "logicTerminated") == 0){
        deleteUE(s);
    }else{
        EV << "ModuleSpawner::signal not recognized" << endl;
    }
}


void ModuleSpawner::finish()
{
    std::cout << "Cars parked at " << simTime() << ": " << PCcounter << endl;
    std::cout << "Ue in coverage at " << simTime() << ": " << UEcounter << endl;
    std::cout << "Timeline: " << timeline << endl;
    std::cout << "Remaining time to park" << endl;
    double parkTime;
    int index;
    double remainingTime;

    std::ofstream datafile;
    datafile.open(filename_.str().c_str(), std::ios::trunc);
    datafile << PCcounter << endl;
//    datafile <<"{";
    for(auto pc : PCs)
    {
        parkTime = pc->getSubmodule("app", 0)->par("parkTime").doubleValue();
        index = std::atoi(pc->getName()+2);
        remainingTime = parkTime + startParkTimes[index-1] - simTime().dbl();
//        std::cout << "Time park remaining for vehicle: " << index << " " << remainingTime << "s" << endl;
        datafile << remainingTime << ",";
    }
//    datafile <<"}"<<endl;

    datafile.close();

}


void ModuleSpawner::finish(cComponent *component, simsignal_t id){
    EV << "ModuleSpawner::finished called" << endl;
}

void ModuleSpawner::subscribedTo(cComponent *component, simsignal_t id){
    EV << "ModuleSpawner::subscribed to " << id << " by " << component << endl;
}

void ModuleSpawner::unsubscribedFrom(cComponent *component, simsignal_t id){
    EV << "ModuleSpawner::unsubscribed from " << id << " by " << component << endl;
}
