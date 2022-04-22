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

package simulation_examples.simulations.extension_test;

import simu5g.nodes.mec.VirtualisationInfrastructureManager.Dynamic.VirtualisationInfrastructureManagerHost;
import simu5g.nodes.mec.VirtualisationInfrastructureManager.Dynamic.VirtualisationInfrastructureManagerDyn;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import simu5g.world.radio.LteChannelControl;
import simu5g.common.binder.Binder;
import simu5g.common.carrierAggregation.CarrierAggregation;
import simu5g.nodes.NR.gNodeB;
import simu5g.nodes.NR.NRUe;
import inet.node.ethernet.Eth10G;
import simu5g.nodes.Upf;


//
// TODO auto-generated type
//
network VIM_test1
{
    submodules:
        routingRecorder: RoutingTableRecorder {
            @display("p=690,104.88;is=s");
        }
        configurator: Ipv4NetworkConfigurator {
            @display("p=690,184.92;is=s");
        }
        channelControl: LteChannelControl {
            @display("p=690,24.84;is=s");
        }
        binder: Binder {
            @display("p=690,264.96;is=s");
        }
        carrierAggregation: CarrierAggregation {
            @display("p=691.38,342.24;is=s");
        }

        gNodeB1: gNodeB {
            @display("p=156,136;is=vl");
        }

        upf: Upf {
            @display("p=301,233");
        }

        viManager: VirtualisationInfrastructureManagerHost {

            @display("p=301,118");
        }
        car: NRUe {
            @display("p=56,363");
        }

    connections allowunconnected:
	
		upf.pppg++ <--> Eth10G <--> gNodeB1.ppp;
		//viManager.pppeENB <--> Eth10G <--> upf.pppg++;
		//viManager.pppENB++ <--> Eth10G <--> upf.pppg++;
//        viManager.pppENB++ <--> Eth10G <--> gNodeB1.pppMEHost;
}
