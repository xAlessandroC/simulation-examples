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

package simulation_examples.simulations.PervasiveVehicularComputingTesting;


import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.ethernet.Eth100G;
import inet.node.ethernet.Eth10G;
import inet.node.ethernet.Eth1G;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;

import simu5g.world.radio.LteChannelControl;
import simu5g.nodes.Upf;
import simu5g.nodes.NR.gNodeB;
import simu5g.nodes.NR.NRUe;
import simu5g.common.carrierAggregation.CarrierAggregation;
import simu5g.common.binder.Binder;
import simu5g.nodes.mec.UALCMP.UALCMP;
import simu5g.nodes.mec.Dynamic.MEO.MecOrchestratorHost;
import simu5g.nodes.mec.Dynamic.MECHostDyn;
import inet.node.inet.Router;
import inet.node.ethernet.EthernetSwitch;
import simu5g.nodes.cars.MecCar;


//
// @author Angelo Feraudo
//
network PervasiveComputingSingleHostNet
{
    parameters:
        int numUe;
        int numParkedCar;

    submodules:
        routingRecorder: RoutingTableRecorder {
            @display("p=50,75;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=566,210;is=s");
        }
        channelControl: LteChannelControl {
            @display("p=569,126;is=s");
        }
        binder: Binder {
            @display("p=566,279;is=s");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=565,346;is=s");
        }
        upf: Upf {
            @display("p=439,188");
        }
        iUpf: Upf {
            @display("p=244,250");
        }
        gNodeB1: gNodeB {
            @display("p=139,340;is=vl");
        }
        
        mecOrchestrator: MecOrchestratorHost {
            @display("p=285,45;is=n;i=device/server");
        }
        mechost1: MECHostDyn { // Testing device (not real MECHost)
            @display("p=139,250;i=device/server");
        }
        ue[numUe]: NRUe {
            @display("p=89,431");
        }
        ualcmp: UALCMP {
            @display("p=439,45");
        }
        router: Router {
            @display("p=439,125");
        }
		// cloud
        server: StandardHost {
            @display("p=572,45;is=n;i=device/server");
        }

        car[numParkedCar]: MecCar {
            @display("p=78,200;is=vs");
        }
    connections allowunconnected:

		// other links
        ualcmp.toMecOrchestrator --> mecOrchestrator.fromUALCMP;
        ualcmp.fromMecOrchestrator <-- mecOrchestrator.toUALCMP;

        // Core links
        server.pppg++ <--> Eth10G <--> router.pppg++;
        //        router.pppg++ <--> Eth10G {  length = 1000m; } <--> upf.pppg++;
        router.pppg++ <--> Eth10G {  length = 1000m; } <--> upf.filterGate;

        ualcmp.ppp++ <--> Eth10G <--> router.pppg++;

        mecOrchestrator.pppg++ <--> Eth10G <--> router.pppg++;

        // Edge links
        upf.pppg++ <--> Eth10G {  length = 1000m; } <--> iUpf.pppg++;
        iUpf.pppg++ <--> Eth10G <--> gNodeB1.ppp;
        mechost1.ppp++ <--> Eth10G <--> iUpf.pppg++;
        mechost1.ppp++ <--> Eth10G <--> gNodeB1.pppMEHost;

}


