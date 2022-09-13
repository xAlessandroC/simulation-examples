
#ifndef SRC_NODES_MODULESPAWNER_MODULESPAWNER_H_
#define SRC_NODES_MODULESPAWNER_MODULESPAWNER_H_

#include <string>
#include <omnetpp.h>
#include <queue>

using namespace omnetpp;

class ModuleSpawner : public cSimpleModule, public cIListener
{

    omnetpp::cMessage* createNewPC_;
    omnetpp::cMessage* createNewUE_;
    omnetpp::cMessage* createNewDebugPC_;
    omnetpp::cMessage* createNewDebugUE_;
    omnetpp::cMessage* burstGeneration_;
    omnetpp::cMessage* initializePark_;

    double intervalStart;
    double intervalBurst;
    double delta;

    // PC parameters
    int PCcounter;
    int PCcreatedcounter;
    std::vector<cModule*> PCs;
    std::queue<double> generationTimesPCs;

    // UE parameters
    int UEcounter;
    int UEcreatedcounter;

    int timeline;
    int parkcapacity_;

    std::vector<cModule*> UEs;
    std::queue<double> generationTimesUEs;

    // average of entering cars in parking area for each hour (rouded up)
    std::vector<double> lambdaValuesVehicles {0.0395, 0.022, 0.0077, 0.0036, 0.0026, 0.007, 0.021, 0.012, 0.018, 0.042, 0.085, 0.143, 0.206, 0.226, 0.218, 0.176, 0.143, 0.205, 0.194, 0.165, 0.118, 0.087, 0.059, 0.043};

    // affluenza media
    std::vector<int> lambdaValuesUEs{101, 48, 43, 26, 38, 70, 161, 263, 285, 273, 258, 261, 254, 256, 255, 265, 266, 283, 283, 281, 253, 197, 157, 144}; // average of entering UE in covered area

    int parkedCars; // indicates amount of currently parked cars
    int initialParkedCars;

    double meanParkTime;
    double stdddevParkTime;

    simsignal_t parkedCars_;
    simsignal_t ues_;


    private:
        void deletePC(const char* PCName);
        void deleteUE(const char* UEName);

    protected:
        virtual void initialize(int stage) override;
        virtual void handleMessage(omnetpp::cMessage *msg) override;
        virtual void initializePark();
        virtual void generateNewPC();
        virtual void generateNewUE();
        virtual void generatePCBurst(int burstDim);
        virtual void generateUEBurst(int burstDim);

        // Listener methods
        virtual void receiveSignal(cComponent *source, simsignal_t signalID, bool b, cObject *details) {};
        virtual void receiveSignal(cComponent *source, simsignal_t signalID, intval_t i, cObject *details) {};
        virtual void receiveSignal(cComponent *source, simsignal_t signalID, uintval_t i, cObject *details) {};
        virtual void receiveSignal(cComponent *source, simsignal_t signalID, double d, cObject *details) {};
        virtual void receiveSignal(cComponent *source, simsignal_t signalID, const SimTime& t, cObject *details) {};
        virtual void receiveSignal(cComponent *source, simsignal_t signalID, const char *s, cObject *details) override;
        virtual void receiveSignal(cComponent *source, simsignal_t signalID, cObject *obj, cObject *details) {};
        virtual void finish(cComponent *component, simsignal_t id) override;
        virtual void subscribedTo(cComponent *component, simsignal_t id) override;
        virtual void unsubscribedFrom(cComponent *component, simsignal_t id) override;

        virtual ~ModuleSpawner();

    public:
        ModuleSpawner();
};



#endif /* SRC_NODES_MODULESPAWNER_MODULESPAWNER_H_ */
