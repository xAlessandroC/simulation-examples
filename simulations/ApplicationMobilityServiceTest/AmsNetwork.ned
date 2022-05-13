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

package simulation_examples.simulations.ApplicationMobilityServiceTest;

import simulation_examples.simulations.ApplicationMobilityServiceTest.FakeMECHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.networklayer.ipv4.RoutingTableRecorder;
import inet.node.ethernet.Eth10G;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;


import simu5g.common.binder.Binder;
import simu5g.nodes.mec.MECPlatform.MECPlatform;



network AmsNetwork
{
    submodules:

        configurator: Ipv4NetworkConfigurator {
            @display("p=139,60");
        }
        binder: Binder {
            @display("p=690,264.96;is=s");
        }

        router: Router {
            @display("p=238,152");
        }

        serviceConsumer: StandardHost {
            @display("p=484,152;is=n;i=device/server");
        }

        mecHost: FakeMECHost {
			// MecPlatform  host
			// It runs available services
            @display("p=63,152;i=device/device");
        }
    connections:
        serviceConsumer.pppg++ <--> Eth10G <--> router.pppg++;
        mecHost.eth++ <--> router.ethg++;
}
