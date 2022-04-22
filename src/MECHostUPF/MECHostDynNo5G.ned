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

package simulation_examples.MECHostUPF;
import simu5g.nodes.mec.VirtualisationInfrastructure.VirtualisationInfrastructure;
import simu5g.nodes.mec.VirtualisationInfrastructureManager.Dynamic.VirtualisationInfrastructureManagerHost;
import simu5g.nodes.mec.MECPlatformManager.Dynamic.MecPlatformManagerHost;
import simu5g.nodes.mec.MECPlatform.MECPlatform;
import simu5g.nodes.mec.MECPlatformManager.MecPlatformManager;
import simu5g.nodes.Upf;
import simu5g.nodes.NR.NRUe;

import inet.node.ethernet.Eth10G;
import inet.applications.contract.IApp;
import inet.linklayer.contract.IEthernetInterface;
import inet.node.inet.Router;
import inet.node.ethernet.EthernetSwitch;
import inet.node.inet.StandardHost;



//
// @author Alessandro Calvio
// @author Angelo Feraudo
//

// This MEC host module has two type of resources:
// - static 
// - non static (this are managed by the Dynamic virutalisation infrastructure Manager)
// Static resources are less if compared with a normal MEC Host
module MECHostDynNo5G
{
    parameters:
        @display("i=device/mainframe;bgb=1036,665");

        // TODO static resources to be defined (now = normal MEC Host)
        //#	MEC Host Resources
        int maxMECApps = default(0);
        double localRam @unit("B") = default(32GB);
        double localDisk @unit("B") = default(100TB);
        double localCpuSpeed = default(300000); 	 // CPU's clock speed in Million Instruction per Seconds (MIPS)

        //# Node specs
        string nodeType = "MECHost";
        string gateway = default("upf");

        int numBGMecApp = default(0);
        string eNBList = default("");


        // Graphic parameters
        string color = default("red");

    gates:
        inout pppENB[];
        inout ethserver;
        inout pppVim;
        inout pppMepm;
//        input radioIn[] @directin; // for d2d communication

    submodules:

        vim: StandardHost {

            @display("p=338.514,164.574;i=device/server");
        }

        mecPlatformManager: StandardHost {

            @display("p=338.514,438.864;i=device/server");
        }

        mecPlatform: MECPlatform {

            @display("p=470.976,313.092;i=device/server");
        }

        router: Router {
            @display("p=338.514,313.092");
        }

        upf_mec: Upf {
            parameters:
                @display("p=234.15001,313.092");
                nodeType = "UPF_MEC";
        }
    connections allowunconnected:
	

        
        vim.pppg++ <--> Eth10G <--> router.pppg++;
        mecPlatformManager.pppg++ <--> Eth10G <--> router.pppg++;
        mecPlatform.virtInfr <--> Eth10G <--> router.ethg++;
		
		router.pppg++ <--> Eth10G <--> upf_mec.pppg++;
		router.pppg++ <--> Eth10G <--> upf_mec.filterGate;
		
        // upf_mec is connected with an antenna and a i-upf
        for i=0..sizeof(pppENB)-1 {
            pppENB++ <--> upf_mec.pppg++;
        }



}
