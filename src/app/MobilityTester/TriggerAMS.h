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

#ifndef __SIMULATION_EXAMPLES_TRIGGERAMS_H_
#define __SIMULATION_EXAMPLES_TRIGGERAMS_H_

#include <omnetpp.h>
#include "inet/transportlayer/contract/tcp/TcpSocket.h"
#include "inet/networklayer/common/L3Address.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "nodes/mec/utils/httpUtils/httpUtils.h"

#include "nodes/mec/MECPlatform/MECServices/packets/HttpRequestMessage/HttpRequestMessage.h"
#include "nodes/mec/MECPlatform/MECServices/packets/HttpResponseMessage/HttpResponseMessage.h"

using namespace omnetpp;

/**
 * TODO - Generated class
 */
class TriggerAMS : public cSimpleModule, public inet::TcpSocket::ICallback
{
  private:
    inet::TcpSocket tcpSocket;
    HttpBaseMessage* currentHttpMessage;
    std::string buffer;

    inet::L3Address localIPAddress;
    int localPort;

    inet::L3Address destIPAddress;
    int destPort;
  protected:
    virtual void initialize(int stage) override;
    virtual int numInitStages() const override { return inet::NUM_INIT_STAGES; }
    virtual void handleMessage(cMessage *msg) override;


    virtual void socketDataArrived(inet::TcpSocket *socket, inet::Packet *packet, bool urgent) override {EV << "Tigger data arrived!"<< endl;};
    virtual void socketAvailable(inet::TcpSocket *socket, inet::TcpAvailableInfo *availableInfo) override{EV << "Trigger::SocketAvailable" << endl;};
    virtual void socketEstablished(inet::TcpSocket *socket) override;
    virtual void socketPeerClosed(inet::TcpSocket *socket) override {socket->close();}
    virtual void socketClosed(inet::TcpSocket *socket) override {EV << "Trigger::connection closed" << endl;}
    virtual void socketFailure(inet::TcpSocket *socket, int code) override {}
    virtual void socketStatusArrived(inet::TcpSocket *socket, inet::TcpStatusInfo *status) override {}
    virtual void socketDeleted(inet::TcpSocket *socket) override {}
};


#endif
