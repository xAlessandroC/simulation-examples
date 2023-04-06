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

package simulation_examples.nodes.moduleSpawner;


simple ModuleSpawner
{
    parameters:

        @display("i=block/wtx");
        
       	double intervalStart = default(0);
       	double intervalBurst = default(10);
       	double delta = default(2);
       	int fileParameters = default(0);
       	int parkCapacity = default(50);
       	int initialParkedCars = default(20);
       	int initialUes = default(0);
       	int initTimeLine = default(0);
       	bool initFromFile = default(false);
       	bool generateFromFile = default(false);
       	
       	@signal[parkedCars](type=long);
    	@statistic[parkedCars](source=parkedCars; record=vector; interpolationmode=none);
    	@signal[ues](type=long);
    	@statistic[ues](source=ues; record=vector; interpolationmode=none);
       	
}
