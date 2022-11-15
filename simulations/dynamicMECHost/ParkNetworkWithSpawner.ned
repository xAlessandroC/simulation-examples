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

package simulation_examples.simulations.dynamicMECHost;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.configurator.ipv4.HostAutoConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.ethernet.Eth10G;
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
import simu5g.nodes.cars.MecCar;

import simulation_examples.nodes.moduleSpawner.ModuleSpawner;

//
// First simulation Network
// @author Angelo Feraudo
// @author Alessandro Calvio
//
network ParkNetworkWithSpawner
{
    parameters:
//        router.hasOspf=true;

    submodules:
        routingRecorder: RoutingTableRecorder {
            @display("p=50,75;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=690,184.92;is=s");
        }
//        router: Router {
//            @display("p=419,81;i=device/smallrouter");
//        }
        channelControl: LteChannelControl {
            @display("p=690,24.84;is=s");
        }
        binder: Binder {
            @display("p=690,264.96;is=s");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=691.38,342.24;is=s");
        }
        spawner: ModuleSpawner {
            @display("p=691,424;is=s");
        }
        upf: Upf {
            @display("p=352,139");
        }
        iUpf: Upf {
            @display("p=352,212");
        }
        mecOrchestrator: MecOrchestratorHost {
            @display("p=439,51;is=n;i=device/server");
        }
        mechost1: MECHostDyn { // Testing device (not real MECHost)
            @display("p=139,147;i=device/server");
        }
        gNodeB1: gNodeB {
            @display("p=139,302;is=vl");
        }
        ualcmp: UALCMP;
        router: Router {
            @display("p=523,138");
        }

        server: StandardHost {
            @display("p=690,139;is=n;i=device/server");
        }

//        templateCar: MecCar {
//
//            @display("p=626,424;is=s");
//        }

    connections allowunconnected:

		// other links
        ualcmp.toMecOrchestrator --> mecOrchestrator.fromUALCMP;
        ualcmp.fromMecOrchestrator <-- mecOrchestrator.toUALCMP;

        // Core links
        server.pppg++ <--> Eth10G <--> router.pppg++;
        router.pppg++ <--> Eth10G {length=1000m;} <--> upf.pppg++;
        router.pppg++ <--> Eth10G {length=1000m;} <--> upf.filterGate;
        ualcmp.ppp++ <--> Eth10G <--> router.pppg++;
        mecOrchestrator.pppg++ <--> Eth10G <--> router.pppg++;

        // Edge links
        upf.pppg++ <--> Eth10G {length=1000m;} <--> iUpf.pppg++;
        iUpf.pppg++ <--> Eth10G <--> gNodeB1.ppp;
        mechost1.ppp++ <--> Eth10G <--> iUpf.pppg++;
        mechost1.ppp++ <--> Eth10G <--> gNodeB1.pppMEHost;

}
