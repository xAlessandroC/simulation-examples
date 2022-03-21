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

#ifndef __SIMU5G_VIMTESTER_H_
#define __SIMU5G_VIMTESTER_H_

#include <omnetpp.h>
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/contract/udp/UdpSocket.h"

// Registration Message
#include "apps/mec/MEOApp/Messages/RegistrationPkt_m.h"

//// mm4 messages
//#include "apps/mec/MEOApp/Messages/MeoVimPackets_m.h"
//
//// mm3 messages
//#include "apps/mec/MEOApp/Messages/MeoMepmPackets_m.h"
#include "apps/mec/MEOApp/Messages/MeoPackets_m.h"


using namespace omnetpp;

/**
 * Testing MEO class
 */
class MEOTester : public cSimpleModule
{
  protected:
    int localPort;
    const char* localAddress;
    int meoPort;
    inet::L3Address meoIPAddress;

    inet::UdpSocket udpSocket;
    ReferencePointType mm;

    // Other methods
    virtual void sendMessage();

    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    virtual void initialize(int stage) override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif
