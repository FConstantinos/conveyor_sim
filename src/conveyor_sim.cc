//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include "ABConveyorConfiguration.h"

using namespace std;
using namespace conveyorsim;

int main(int argc, char* argv[]) {
    string usage = ""
                   "usage: conveyor_sim [-h] [-n timeslots] [-c capacity] [-d duration] [-v]\n"
                   "\n"
                   "A simulation of a conveyor belt that conveys items which workers on either\n"
                   "side of the belt can collect and assemble products. At the beginning of each\n"
                   "timeslot, either an 'A' item, a 'B' item or no item are chosen with uniform\n"
                   "probability. The chosen item is then enqueued on the left end of a conveyor\n"
                   "belt that moves from left to right. The conveyor belt has a given capacity;\n"
                   "a number of positions that can hold an item on its surface. The belt moves\n"
                   "one position per timeslot. Workers occupy both ends of those positions.\n"
                   "Workers possess two arms with which they can collect items from the belt.\n"
                   "Once they have collected both types of items from the belt, they can start\n"
                   "assembling an item 'P'. The assembly takes a given duration of time. After\n"
                   "the assembly is complete, the 'A' and 'B' items used for its assembly are\n"
                   "discarded from the arms of the worker and a 'P' item is deposited on one\n"
                   "of the arms of the worker to be emplaced on the conveyor belt at the first\n"
                   "opportunity. At each timeslot only one item can be collected or emplaced on\n"
                   "any single position of the conveyor belt. After the simulation is over, the\n"
                   "number of assembled products and unused items that made it through the belt\n"
                   "is returned.\n"
                   "\n"
                   "optional arguments:\n"
                   "-h              show this help message and exit\n"
                   "-n timeslots    number of timeslots to run the simulation (default = 1)\n"
                   "-c capacity     capacity of the conveyor belt; how many items it can carry (default = 1)\n"
                   "-d duration     assembly duration in timeslots (default = 0)\n"
                   "-v              verbose; print information about the simulation at the end of each timeslot\n";

    size_t numSlots = 1;
    size_t convSize = 1;
    size_t assemblyDuration = 0;

    bool verbose = false;

    for(;;) {
        switch(getopt(argc, argv, "hn:c:d:v")) {
            case 'h':
                cout << usage << endl;
                return 0;
            case 'n':
                numSlots = atoi(optarg);
                continue;
            case 'c':
                convSize = atoi(optarg);
                continue;
            case 'v':
                verbose = true;
                continue;
            case 'd':
                assemblyDuration = atoi(optarg);
                continue;
            default:
                cout << usage << endl;
                return 0;
            case -1:
                break;
        }
        break;
    }

    if(!numSlots || !convSize) {
        return 0;
    }

    ABConveyorConfiguration sim(convSize, assemblyDuration);

    if (verbose) {
        for (size_t slot = 0; slot < numSlots; slot++) {
            sim.run(1);
            cout << sim << endl;
        }
    } else {
        sim.run(numSlots);
    }

    cout << "Product count: " << sim.getProductCount() << endl;
    cout << "Drop count: " << sim.getDropCount() << endl;

    return 0;
}
