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

#include "AddressResolvingApp.h"

Define_Module(AddressResolvingApp);

void AddressResolvingApp::initialize(int stage)
{
    if (stage!=inet::INITSTAGE_APPLICATION_LAYER-1){
        return;
    }
    cMessage *msg = new cMessage("connect");
    scheduleAt(simTime()+0.01, msg);

}

void AddressResolvingApp::handleMessage(cMessage *msg)
{
    if(msg->isSelfMessage())
    {
        inet::L3Address address = inet::L3AddressResolver().resolve(par("brokerAddress").stringValue());
        EV << "BrokerAddressResolved!!! " << address.str() << endl;
    }
}
