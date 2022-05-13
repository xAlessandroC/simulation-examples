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
import simu5g.nodes.mec.MECPlatform.MECPlatform;
import inet.node.ethernet.Eth10G;

module FakeMECHost
{
    gates:
        inout eth[];
    submodules:
        mecPlatform: MECPlatform {
            parameters:
                @display("p=573.535,158.12999");
        }
   	connections allowunconnected:
   	    eth++ <--> Eth10G <--> mecPlatform.virtInfr;
   	    
}
