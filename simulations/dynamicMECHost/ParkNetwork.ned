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
// First simulation Network
// @author Angelo Feraudo
// @author Alessandro Calvio
//
network ParkNetwork
{
    parameters:
        int numUe;
        int numParkedCar;

    submodules:
        routingRecorder: RoutingTableRecorder {
            @display("p=50,75;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=966,229;is=s");
        }
        //        router: Router {
        //            @display("p=419,81;i=device/smallrouter");
        //        }
        channelControl: LteChannelControl {
            @display("p=690,24.84;is=s");
        }
        binder: Binder {
            @display("p=966,302;is=s");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=966,385;is=s");
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
        ue[numUe]: NRUe {
            @display("p=89,431");
        }
        ualcmp: UALCMP {
            @display("p=586,51");
        }
        router: Router {
            @display("p=523,138");
        }
		// cloud
        router2: Router {
            @display("p=815,135");
        }
        datacenterSwitch: EthernetSwitch {
            @display("p=935,137");
        }

        cloudServer: StandardHost {
            @display("p=935,68;is=n;i=device/server");
        }
        server: StandardHost {
            @display("p=523,212;is=n;i=device/server");
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
        router.pppg++ <--> Eth10G {  length = 1000m; } <--> upf.pppg++;
        router.pppg++ <--> Eth10G {  length = 1000m; } <--> upf.filterGate;

        ualcmp.ppp++ <--> Eth10G <--> router.pppg++;

        router.pppg++ <--> Eth100G {  length = 1000km; } <--> router2.pppg++;
        router2.ethg++ <--> Eth1G <--> datacenterSwitch.ethg++;
        datacenterSwitch.ethg++ <--> Eth1G <--> cloudServer.ethg++;
        mecOrchestrator.pppg++ <--> Eth10G <--> router.pppg++;

        // Edge links
        upf.pppg++ <--> Eth10G {  length = 1000m; } <--> iUpf.pppg++;
        iUpf.pppg++ <--> Eth10G <--> gNodeB1.ppp;
        mechost1.ppp++ <--> Eth10G <--> iUpf.pppg++;
        mechost1.ppp++ <--> Eth10G <--> gNodeB1.pppMEHost;

}
