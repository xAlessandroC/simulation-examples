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

package simulation_examples.simulations.MultipleDynamicMECHosts;

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
network MultipleDynMecHostsNet
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
        channelControl: LteChannelControl {
            @display("p=965,36;is=s");
        }
        binder: Binder {
            @display("p=966,302;is=s");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=966,385;is=s");
        }
        upf: Upf {
            @display("p=439,188");
        }
        iUpf: Upf {
            @display("p=244,250");
        }
        iUpf2: Upf {
            @display("p=626,250");
        }
        mecOrchestrator: MecOrchestratorHost {
            @display("p=285,45;is=n;i=device/server");
        }
        mechost1: MECHostDyn { // Testing device (not real MECHost)
            @display("p=139,250;i=device/server");
        }

        mechost2: MECHostDyn { // Testing device (not real MECHost)
            @display("p=724,250;i=device/server");
        }

        gNodeB1: gNodeB {
            @display("p=139,340;is=vl");
        }

        gNodeB2: gNodeB {
            @display("p=724,340;is=vl");
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
        router2: Router {
            @display("p=642,125");
        }
        datacenterSwitch: EthernetSwitch {
            @display("p=777,124");
        }

        cloudServer: StandardHost {
            @display("p=777,45;is=n;i=device/server");
        }
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

        router.pppg++ <--> Eth100G {  length = 1000km; } <--> router2.pppg++;
        router2.ethg++ <--> Eth1G <--> datacenterSwitch.ethg++;
        datacenterSwitch.ethg++ <--> Eth1G <--> cloudServer.ethg++;
        mecOrchestrator.pppg++ <--> Eth10G <--> router.pppg++;

        // Edge links
        upf.pppg++ <--> Eth10G {  length = 1000m; } <--> iUpf.pppg++;
        upf.pppg++ <--> Eth10G {  length = 1000m; } <--> iUpf2.pppg++;
        iUpf.pppg++ <--> Eth10G <--> gNodeB1.ppp;
        iUpf2.pppg++ <--> Eth10G <--> gNodeB2.ppp;
        mechost1.ppp++ <--> Eth10G <--> iUpf.pppg++;
        mechost1.ppp++ <--> Eth10G <--> gNodeB1.pppMEHost;

        mechost2.ppp++ <--> Eth10G <--> iUpf2.pppg++;
        mechost2.ppp++ <--> Eth10G <--> gNodeB2.pppMEHost;

        gNodeB1.x2++ <--> Eth10G {  length = 800m; } <--> gNodeB2.x2++;
}
