//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "TestRandomNumber.h"


Define_Module(TestRandomNumber);

void TestRandomNumber::initialize()
{

    parkcapacity_ = par("parkCapacity").intValue();
    std::string fileName = "cars_" + std::to_string(parkcapacity_) + ".txt";
    file_cars.open(fileName.c_str());
    file_ues.open("ues.txt");

    cMessage *msg = new cMessage("myMessage");
    scheduleAt(0.01, msg);
}

void TestRandomNumber::handleMessage(cMessage *msg)
{

    if(std::strcmp(msg->getName(),"myMessage") == 0)
    {
        double lambda_cars;
        int burst_cars_dim;
        int burst_ue_dim;
        for(auto it : lambdaValuesVehicles)
        {
            lambda_cars = it * parkcapacity_;
            burst_cars_dim = static_cast<int>(poisson(lambda_cars));

            file_cars << burst_cars_dim << endl;
        }
        for(auto it : lambdaValuesUEs)
        {

            burst_ue_dim = static_cast<int>(poisson(it)); // poisson distribution ues

            file_ues << burst_ue_dim << endl;
        }


    }

    delete msg;
}

void TestRandomNumber::finish()
{
    file_cars.close();
    file_ues.close();
}
