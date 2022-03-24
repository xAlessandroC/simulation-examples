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

package simulation_examples.simulations.OrchestratorSimu2;

import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
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
import simu5g.apps.mec.MEOApp.MecOrchestratorHost;
import simu5g.nodes.mec.Dynamic.MECHostDyn;

network RealOrchNetwork
{
    submodules:
        routingRecorder: RoutingTableRecorder {
            @display("p=50,75;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=50,125");
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
        upf: Upf {
            @display("p=352,139");
        }
        iUpf: Upf {
            @display("p=352,212");
        }
        mecOrchestrator: MecOrchestratorHost {
            @display("p=248,52;is=n;i=device/server");
        }
        mechost1: MECHostDyn { // Testing device (not real MECHost)
            @display("p=139,212;i=device/server");
        }
        //        mecHost2: StandardHost { // Testing device (not real MECHost)
        //            @display("p=352,349;is=n;i=device/server");
        //        }
        gNodeB1: gNodeB {
            @display("p=139,302;is=vl");
        }
        ue: NRUe {
            @display("p=237.64375,310.45377");
        }
//        ue2: NRUe {
//            @display("p=200,310");
//        }
        ualcmp: UALCMP {
            @display("p=470,51;i=device/smallrouter");
        }
    connections allowunconnected:
        upf.pppg++ <--> Eth10G <--> iUpf.pppg++;
        iUpf.pppg++ <--> Eth10G <--> gNodeB1.ppp;
        ualcmp.ppp++ <--> Eth10G <--> upf.filterGate;

        ualcmp.toMecOrchestrator --> mecOrchestrator.fromUALCMP;
        ualcmp.fromMecOrchestrator <-- mecOrchestrator.toUALCMP;

        upf.pppg++ <--> Eth10G <--> mecOrchestrator.ppp++;
        mechost1.pppVim <--> Eth10G <--> iUpf.pppg++;
        mechost1.pppMepm <--> Eth10G <--> iUpf.pppg++;
        //        mecHost2.pppg++ <--> Eth10G <--> iUpf.pppg++;
        mechost1.pppENB++ <--> Eth10G <--> gNodeB1.pppMEHost;

}
