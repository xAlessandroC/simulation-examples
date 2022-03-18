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

package simulation_examples.simulations.MecPlatform_simulationtest;

import simu5g.nodes.mec.VirtualisationInfrastructureManager.Dynamic.VirtualisationInfrastructureManagerHost;
import simu5g.nodes.mec.VirtualisationInfrastructureManager.Dynamic.VirtualisationInfrastructureManagerDyn;
import simu5g.nodes.mec.Dynamic.MECHostDyn;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import simu5g.world.radio.LteChannelControl;
import simu5g.common.binder.Binder;
import inet.node.inet.Router;
import simu5g.common.carrierAggregation.CarrierAggregation;
import simu5g.nodes.NR.gNodeB;
import simu5g.nodes.NR.NRUe;
import simu5g.nodes.cars.MecCar;
import inet.node.inet.StandardHost;
import inet.node.ethernet.Eth10G;
import simu5g.nodes.Upf;


network MecPlatformSimulationNetwork
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
            @display("p=72,256;is=vl");
        }
        
        mechost: MECHostDyn {
            @display("p=407,118");
        }
        
        router: Router {
            @display("p=289,117;i=device/smallrouter");
        }
        upf: Upf {
            @display("p=180,118");
        }
        iUpf: Upf {
            @display("p=72,118");
        }
        server: StandardHost {
            @display("p=289,33;is=n;i=device/server");
        }
        
	connections allowunconnected:
	    mechost.ethserver <--> Eth10G <--> router.ethg++;
        server.pppg++ <--> Eth10G <--> router.pppg++;
        router.pppg++ <--> Eth10G <--> upf.filterGate;
        upf.pppg++ <--> Eth10G <--> iUpf.pppg++;
        iUpf.pppg++ <--> Eth10G <--> gNodeB1.ppp;
}
