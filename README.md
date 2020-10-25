# Introduction 
This is a simple simulation of a conveyor belt that conveys items which workers on either side of the belt can collect 
and assemble products. It returns the number of assembled products and unused items that made it through the belt.
   
# Build and Installation

## Requirements
 * A linux environment
 * boost >= 1.71
 * cmake >= 3.16
 * doxygen >= 1.8.17
   * for building Doxygen style documentation
 * graphviz >= 2.42.2-3build2
   * for building documentation
 * googletest >= 1.10
   * for building unit tests
      
## Instructions
 * Clone this repository to your local machine
 * run the following commands:
    * cd /your/clone/directory
    * mkdir build && cd build
    * cmake -DCMAKE_INSTALL_PREFIX=/your/install/directory ..
        * to be able to build documentation, add "-DBUILD_DOCUMENTATION=ON" (without quotes)
        * to be able to build the tests, add "-DENABLE_TEST=ON" (without quotes)
    * make all 
        * to build everything
    * make doc
        * to build the Doxygen documentation
        * open *index.html* under the /doc folder to view the documentation
    * make conveyor_sim
        * to build the **conveyor_sim** application
    * make conveyor_sim_test
        * to build the unit tests
        * to run the unit tests, simply run the conveyor_sim_test executable
    * make install
        * to install the **conveyor_sim** application
        * will be under /your/install/directory/bin
        
# Usage
````
usage: conveyor_sim [-h] [-n timeslots] [-c capacity] [-d duration] [-v]

A simulation of a conveyor belt that conveys items which workers on either
side of the belt can collect and assemble products. At the beginning of each
timeslot, either an 'A' item, a 'B' item or no item are chosen with uniform
probability. The chosen item is then enqueued on the left end of a conveyor
belt that moves from left to right. The conveyor belt has a given capacity;
a number of positions that can hold an item on its surface. The belt moves
one position per timeslot. Workers occupy both ends of those positions.
Workers possess two arms with which they can collect items from the belt.
Once they have collected both types of items from the belt, they can start
assembling an item 'P'. The assembly takes a given duration of time. After
the assembly is complete, the 'A' and 'B' items used for its assembly are
discarded from the arms of the worker and a 'P' item is deposited on one
of the arms of the worker to be emplaced on the conveyor belt at the first
opportunity. At each timeslot only one item can be collected or emplaced on
any single position of the conveyor belt. After the simulation is over, the
number of assembled products and unused items that made it through the belt
is returned.

optional arguments:
-h              show this help message and exit
-n timeslots    number of timeslots to run the simulation (default = 1)
-c capacity     capacity of the conveyor belt; how many items it can carry (default = 1)
-d duration     assembly duration in timeslots (default = 0)
-v              verbose; print information about the simulation at the end of each timeslot
````

# Examples

100 timeslots, 3 pairs of workers and 4 timeslots assembly duration:
````
./conveyor_sim -n 100 -c 3 -d 4
Product count: 22
Drop count: 6
````

100000 timeslots, 60 pairs of workers and 4 timeslots assembly duration:
````
./conveyor_sim -n 100000 -c 60 -d 4
Product count: 33154
Drop count: 126
````