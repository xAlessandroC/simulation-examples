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

#ifndef __SIMULATION_EXAMPLES_TESTRANDOMNUMBER_H_
#define __SIMULATION_EXAMPLES_TESTRANDOMNUMBER_H_

#include <omnetpp.h>
#include <fstream>

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class TestRandomNumber : public cSimpleModule
{
   // average of entering cars in parking area for each hour (rouded up)
   std::vector<double> lambdaValuesVehicles {0.0395, 0.022, 0.0077, 0.0036, 0.0026, 0.007, 0.021, 0.012, 0.018, 0.042, 0.085, 0.143, 0.206, 0.226, 0.218, 0.176, 0.143, 0.205, 0.194, 0.165, 0.118, 0.087, 0.059, 0.043};

   // affluenza media
   std::vector<int> lambdaValuesUEs{101, 48, 43, 26, 38, 70, 161, 263, 285, 273, 258, 261, 254, 256, 255, 265, 266, 283, 283, 281, 253, 197, 157, 144}; // average of entering UE in covered area

   int parkcapacity_;

   std::ofstream file_cars;
   std::ofstream file_ues;



  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

#endif
