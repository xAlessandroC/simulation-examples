
#ifndef SRC_NODES_MODULESPAWNER_MODULESPAWNER_H_
#define SRC_NODES_MODULESPAWNER_MODULESPAWNER_H_

#include <string>
#include <omnetpp.h>
#include <queue>
#include <iostream>
#include <fstream>

#define UES 0
#define PARKEDCARS 1

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

    bool initFromFile;

    // Generate ues and parked cars by reading a file
    bool generateFromFile;
    std::vector<float> cars;
    std::vector<int> ues;

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

    std::vector<double> startParkTimes;

    std::stringstream filename_;

//    cRNG *crngUes;
//    cRNG *crngCars;
//    cMarsenneTwister *twisterUe;
//    cMarsenneTwister *twisterCars;


//    std::vector <double> initParkTimes {5980.12, 1564.98, 7742.84, 25027.3, 25436.4}; // from 0 to 21600
//    std::vector<double> initParkTimes {542.1, 17827.1, 18236.1, 11799.9}; // from 21600 to 28800
//    std::vector<double> initParkTimes {14227.1, 14636.1, 8199.1}; //from 28800 to 32400
//    std::vector<double> initParkTimes {10627.1, 11036.1, 4599.1, 13542.7, 14272.9, 16966, 8625.66, 12940.7, 9129.83, 4746.51}; //from 32400 to 36000
//    std::vector<double> initParkTimes {7027.1, 7436.1, 999.1, 9942.1, 10672.1, 13366.1, 5025.1, 9340.1, 5529.1, 1146.1, 3672.18, 20818.8, 27593.9, 8382.98, 24914.8, 8703.9, 15686.1, 4012.55, 14577.6, 9224.81}; // from 36000 to 39600
//    std::vector<double> initParkTimes {3427.1, 3836.1, 6342.1, 7072.1, 9766.1, 1425.1, 5740.1, 1929.1, 72.1, 17218.1, 23993.1, 4782.1, 21314.1, 5103.1, 12086.1, 412.1, 10977.1, 5624.1, 1883.38, 2952.44, 29785.9, 228.306, 16424.8, 14965.5, 14451, 19734.9, 12810.4, 12155.7, 699.885, 17684.9, 9293.41}; // from 39600 to 43200
//    std::vector<double> initParkTimes {236.1, 2742.1, 3472.1, 6166.1, 2140.1, 13618.1, 20393.1, 1182.1, 17714.1, 1503.1, 8486.1, 7377.1, 2024.1, 26185.1, 12824.1, 11365.1, 10851.1, 16134.1, 9210.1, 8555.1, 14084.1, 5693.1, 17592.4, 18961.6, 14762.2, 9344.29, 9382.79, 20829.6, 6655.57, 2936.26, 7974.21, 16887.4, 2293.7, 8926.24, 7252.96, 2893.6, 9455.37, 8035.19, 17297.3, 10581.1, 17599.9}; // from  43200 to 46800
//    std::vector<double> initParkTimes {2566.1, 10018.1, 16793.1, 14114.1, 4886.1, 3777.1, 22585.1, 9224.1, 7765.1, 7251.1, 12534.1, 5610.1, 4955.1, 10484.1, 2093.1, 13992.1, 15361.1, 11162.1, 5744.1, 5782.1, 17229.1, 3055.1, 4374.1, 13287.1, 5326.1, 3652.1, 5855.1, 4435.1, 13697.1, 6981.1, 13999.1, 14086.7, 12724.1, 13271.2, 7635.49, 10879.9, 11013.1, 13942.4, 20537, 9078.57, 5294.18, 8987.46, 18337.6}; // from  46800 to 50400
//    std::vector<double> initParkTimes {6418.1, 13193.1, 10514.1, 1286.1, 177.1, 18985.1, 5624.1, 4165.1, 3651.1,
//        8934.1, 2010.1, 1355.1, 6884.1, 10392.1, 11761.1, 7562.1, 2144.1, 2182.1, 13629.1, 774.1, 9687.1,
//        1726.1, 52.1, 2255.1, 835.1, 10097.1, 3381.1, 10399.1, 10486.1, 9124.1, 9671.1, 4035.1, 7279.1, 7413.1,
//        10342.1, 16937.1, 5478.1, 1694.1, 5387.1, 14737.1, 8341.06, 13756, 18041.9, 17423.7, 8442.27, 2133.25,
//        9308.06, 15538.5, 4251.67, 20247.7, 9180.57, 12487, 17499.2, 374.804, 3461.98, 14874.2, 21377.7, 16230.2,
//        24867.8, 2386.91, 17404.4, 6117.54, 24072.3, 6590.94, 2499.16, 24660.8, 15013.8, 3342.3, 5321.12}; // from 50400 to 54000
//    std::vector<double> initParkTimes {2818.1, 9593.1, 6914.1, 15385.1, 2024.1, 565.1, 51.1, 5334.1, 3284.1, 6792.1,
//        8161.1, 3962.1, 10029.1, 6087.1, 6497.1, 6799.1, 6886.1, 5524.1, 6071.1, 435.1, 3679.1, 3813.1, 6742.1, 13337.1,
//        1878.1, 1787.1, 11137.1, 4741.1, 10156.1, 14441.1, 13823.1, 4842.1, 5708.1, 11938.1, 651.1, 16647.1, 5580.1,
//        8887.1, 13899.1, 11274.1, 17777.1, 12630.1, 21267.1, 13804.1, 2517.1, 20472.1, 2990.1, 21060.1, 11413.1, 1721.1,
//        34936.8, 14418.2, 27852.4, 15457.3, 5005.33, 5287.4, 9193.17, 14708.6, 25746.1, 14598.5, 6795.64};// from 54000 to 57600
//
//    std::vector<double> initParkTimes {5993.1, 3314.1, 11785.1, 1734.1, 3192.1, 4561.1, 362.1, 6429.1, 2487.1, 2897.1,
//        3199.1, 3286.1, 1924.1, 2471.1, 79.1, 213.1, 3142.1, 9737.1, 7537.1, 1141.1, 6556.1, 10841.1, 10223.1, 1242.1,
//        2108.1, 8338.1, 13047.1, 1980.1, 5287.1, 10299.1, 7674.1, 14177.1, 9030.1, 17667.1, 10204.1, 16872.1, 17460.1,
//        7813.1, 31336.1, 10818.1, 24252.1, 11857.1, 1405.1, 1687.1, 5593.1, 11108.1, 22146.1, 10998.1, 3195.1, 6050.32,
//        13596, 23206.2, 15082.3, 6416.84, 4656.47, 337.88, 6963.72, 1836.6, 93.9687, 18407.4, 16944.2, 15968.3, 14495.6,
//        17788.6}; // from 57600 to 61200
//    std::vector<double> initParkTimes {2393.1, 8185.1, 961.1, 2829.1, 6137.1, 3937.1, 2956.1, 7241.1, 6623.1, 4738.1,
//        9447.1, 1687.1, 6699.1, 4074.1, 10577.1, 5430.1, 14067.1, 6604.1, 13272.1, 13860.1, 4213.1, 27736.1, 7218.1,
//        20652.1, 8257.1, 1993.1, 7508.1, 18546.1, 7398.1, 2450.1, 9996.1, 19606.1, 11482.1, 2816.1, 1056.1, 3363.1,
//        14807.1, 13344.1, 12368.1, 10895.1, 14188.1, 11939.8, 3048.89, 16663.4, 7690.88, 3575.54, 18320.3, 9420.13,
//        11624.6, 20011.2, 11138, 16654.3, 15896.8, 18441.2, 14034.9, 7279.33, 2934.95};// from 61200 to 64800
//    std::vector<double> initParkTimes{4585.1, 2537.1, 337.1, 3641.1, 3023.1, 1138.1, 5847.1, 3099.1, 474.1, 6977.1,
//        1830.1, 10467.1, 3004.1,  9672.1, 10260.1, 613.1, 24136.1, 3618.1, 17052.1, 4657.1, 3908.1, 14946.1, 3798.1, 6396.1, 16006.1,
//        7882.1, 11207.1, 9744.1, 8768.1, 7295.1, 10588.1, 8339.1, 13063.1, 4090.1, 14720.1, 5820.1, 8024.1,
//        16411.1, 7538.1, 13054.1, 12296.1, 14841.1, 10434.1, 3679.1, 2674.61, 20406.4, 4020.74, 8640.17, 16690.7,
//        13450.2, 1504.42, 12539.6, 12784.4, 13866.5, 16456.7}; // from 64800 to 68400
//    std::vector<double> initParkTimes{985.1, 41.1, 2247.1, 3377.1, 6867.1, 6072.1, 6660.1, 20536.1, 18.1, 13452.1,
//        1057.1, 308.1, 11346.1, 198.1, 2796.1, 12406.1, 4282.1, 7607.1, 6144.1, 5168.1, 3695.1, 6988.1, 4739.1,
//        9463.1, 490.1, 11120.1, 2220.1, 4424.1, 12811.1, 3938.1, 9454.1, 8696.1, 11241.1, 6834.1, 79.1, 16806.1,
//        420.1, 5040.1, 13090.1, 9850.1, 8939.1, 9184.1, 10266.1, 12856.1, 17606.4, 15709.1, 15891.1, 19142.9, 11065.4,
//        5938.05, 8215.78, 7496.06, 11844.2, 9000.21, 17252.3, 9030.79, 15861.1, 11737.6, 7984.67, 4814.62}; // from 68400 to 72000
//
//    std::vector<double> initParkTimes{3267.1, 2472.1, 3060.1, 16936.1, 9852.1, 7746.1, 8806.1, 682.1, 4007.1, 2544.1,
//        1568.1, 95.1, 3388.1, 1139.1, 5863.1, 7520.1, 824.1, 9211.1, 338.1, 5854.1, 5096.1, 7641.1, 3234.1, 13206.1,
//        1440.1, 9490.1, 6250.1, 5339.1, 5584.1, 6666.1, 9256.1, 14006.1, 12109.1, 12291.1, 15542.1, 7465.1, 2338.1,
//        4615.1, 3896.1, 8244.1, 5400.1, 13652.1, 5430.1, 12261.1, 8137.1, 4384.1, 1214.1, 23178.9, 1683.51, 5024.03,
//        16195.7, 2155.54, 12918.9, 23903.9, 15251.5, 17672.3, 4840.87, 3528.6}; // from 72000 to 75600

//    std::vector<double> initParkTimes{13336.1, 6252.1, 4146.1, 5206.1, 407.1, 2263.1, 3920.1, 5611.1, 2254.1, 1496.1,
//        4041.1, 9606.1, 5890.1, 2650.1, 1739.1, 1984.1, 3066.1, 5656.1, 10406.1, 8509.1, 8691.1, 11942.1, 3865.1, 1015.1,
//        296.1, 4644.1, 1800.1, 10052.1, 1830.1, 8661.1, 4537.1, 784.1, 19578.1, 1424.1, 12595.1, 9318.1, 20303.1, 11651.1,
//        14072.1, 1240.1, 11921.3, 17576, 20365, 13712.2, 1155.77, 8380.44, 13039.2, 21811.8, 6766.11}; // from 75600 to 79200

//    std::vector<double> initParkTimes{9736.1, 2652.1, 546.1, 1606.1, 320.1, 2011.1, 441.1, 6006.1, 2290.1, 2056.1, 6806.1,
//        4909.1, 5091.1, 8342.1, 265.1, 1044.1, 6452.1, 5061.1, 937.1, 15978.1, 8995.1, 5718.1, 16703.1, 8051.1, 10472.1,
//        8321.1, 13976.1, 16765.1, 10112.1, 4780.1, 9439.1, 18211.1, 3166.1, 15581.7, 27055.2}; // from 79200 to 82800

    std::vector<double> initParkTimes;

    int parkedCars; // indicates amount of currently parked cars
    int initialParkedCars;
    int initialUes;

    double meanParkTime;
    double stdddevParkTime;

    simsignal_t parkedCars_;
    simsignal_t ues_;

    cModule* vimApp;

    private:
        void deletePC(const char* PCName);
        void deleteUE(const char* UEName);
        void initializeFromFile();
        void includeMecHost(bool include);

    protected:
        virtual void initialize(int stage) override;
        virtual void handleMessage(omnetpp::cMessage *msg) override;
        virtual void finish() override;
        virtual void initializePark();
        virtual void initializeUes();
        virtual void loadGenerators();
        virtual void generateNewPC(double initParkTime=-1);
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
