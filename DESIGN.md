# Design
**conveyor_sim** is a simple simulation of a conveyor belt that conveys items which workers on either side of the belt 
can collect and assemble products. It returns the number of assembled products and unused items that made it through the 
belt. **conveyor_sim**  is build with ease of installation, ease of use, code flexibility and scalability in mind.
Effort is therefore concentrated on packaging, interfacing, documentation and a scalable design with scalable data
structures, algorithms and OOP design.

# Requirements
**conveyor_sim** needs to be able to perform the following simulation:
 - User Input:
    - number of timeslots to run the simulation (default = 1)
    - capacity of the conveyor belt; how many items it can carry (default = 1)
    - assembly duration of the product item in timeslots (default = 0)
 - Setup:
    - Conveyor Belt
        - moves from left to right, one position per timeslot
    - Workers
        - two workers per conveyor belt position
            - one for each side of the position
        - two arms per worker
            - arms can hold items
        - workers can collect 'A' or 'B' items
          - if enough arms are available
        - workers assemble a 'P' item when both 'A' and 'B' have been collected
        - workers cannot collect new items while assembly takes place
        - only a single item can be collected by a worker or placed by a worker on each position per timeslot
            - but not both at the same time
            - only one worker can perform one of the above actions per timeslot
                - priority per timeslot is chosen at random
                    - to avoid deadlocks from possible adversarial item generation
    - An 'A', 'B' or no item are placed on the left of the conveyor with uniform random probability
        - That takes no time and can be used by the workers at the same timeslot it was placed
 - Output:
    - number of unused 'A' or 'B' items that made it through the belt at the end of the simulation
    - number of produced 'P' items that made it through the belt at the end of the simulation
 - Design Flexibility:
    - code design should be flexible enough allow writing more complex future simulations with more complex workers,
      conveyor belts, item generation and worker configurations
 - Ease of Use and Installation:
    - documentation should be provided to the user to easily build, install and use the application
    
# Interface:
Users will interface to the simulation through a command line. Users will be able to set the parameters to the 
simulation at start time. A -h command line option should result in the display of all options and arguments.
A typical -h output should look like this:

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

A display of statistics after the simulation should look like this:
````
Product count: 33154
Drop count: 126
````

# Details
This section will go into some details of the design with respect to the algorithms, data structures and OOP design 
principles used. For a detailed class structure, please generate the doxygen documentation and visit the related
section under Classes. 

## Algorithms
The most complex algorithm used for this work is the interaction of a worker with its assigned position on the conveyor
belt for every timeslot. There are four parts to it that take place in the order given:

 - Try to collect an item if:
    - there is an item to collect on the position
    - the item can be used for the assembly
    - there are enough unused arms
    - the worker is not busy assembling a product
    - the position is not reserved by the opposite worker
 - Try to start the assembly of an item if:
    - it has the items needed to start the assembly
    - the worker is not busy assembling a product
 - Finish the assembly of the item
    - discard component items used for the assembly
    - place the product on one of the worker's arms
 - Release the product item on the conveyor belt if:
    - there is a product item on one of its arms
    - the position is not reserved by the opposite worker
    - the position is empty

## Data Structures
The most complex data structure used for this work is that of the conveyor belt simulation. The conveyor belt is 
implemented as circular buffer of constant capacity. This is because all useful operations (add/remove/access an element
as well as rotate the data structure) are done in O(1) time complexity and O(N) space complexity, where N is the 
capacity of the conveyor belt. The boost::circular_buffer container is used for that, since the STL lacks such 
functionality. Unfortunately, the boost::circular_buffer is not thread safe, which would have been a good attribute for
this simulation since it would mean that we could have multiple workers adding/removing items on the belt concurrently. 
Providing an implementation of a circular buffer that is thread safe is left as future work.

## Interfaces
Most of the simulation components are coded into interfaces (i.e C++ abstract classes). This helps with regard to code
flexibility in the following ways:
 - loose coupling of classes
 - less compile time dependencies
 - easier testing 
    - components use the interfaces only
    - these interfaces can be implemented by a mock
 - new implementations to the abstract classes can be added without breaking relations with other classes
    - for example a conveyor belt implemented with a different data structure other than a circular buffer
    - a random item generator with other than uniform generation probability

Notably, the Worker and ABConveyorConfiguration classes where chosen to not abstract into an interface. This is done
for two reasons:
 - both are covered, at least partially, by the SimulationComponentIF interface
 - there is a wide range of possible generalizations
    - workers that can work on any part of the belt
    - workers with specialized arms for each item
    - many algorithms could be used for the interaction between the opposite workers
    - unless more requirements are provided, it is not clear how those components could generalize
    
Also notably, although the only required information for an item throughout the simulation is an item's part number,
(which can be a single integer number) this information is encapsulated into an ItemPN class which in turn is
encapsulated into an Item class. This complexity is justified for code flexibility for two reasons:
 - it enforces strong typing
 - it allows future implementations to add members to those classes (like weight of item, quality, temperature etc) that
   can affect the simulation without having to change every component that is using them.

## PIMPL idiom
The PIMPL idiom is a C++ programming technique used to hide private members of a class from its header file. Check
[here](https://en.cppreference.com/w/cpp/language/pimpl) for more details. The PIMPL idiom helps with regard to code
flexibility in the following ways:
 - loose coupling of classes
 - less compile time dependencies

# Testing
The Conveyor Belt, Worker and UniformRandomItemGenerator simulation components are unit tested. These are the most
complex of the simulation components. The unit tests make sure that those components function as intended in isolation.
Googletest is used as the testing framework.

# Ideas for Future Work:
 - Thread safe circular buffer data structure where multiple 
   threads can write on different positions at the same time
   - Similar to python's deque with the maxlen argument.
 - Try more implementations of the random generator.
 - Logging mechanism that logs events in the simulation.
 - More testing (see TODOs under the unittests folder).
 - Abstract away the algorithm used by the workers on each side
   to access the position on the conveyor belt and test multiple
   algorithms for performance or ratio of dropped items to total
   items.
