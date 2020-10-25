//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include <gtest/gtest.h>
#include <iostream>
#include "Worker.h"
#include "ConveyorPositionController.h"
#include "ConveyorBelt.h"

using namespace std;
using namespace conveyorsim;

struct WorkerTestCase {
    const size_t capacity;
};

class WorkerTestFixture : public ::testing::Test {

    // run enough timeslots to empty the belt and the worker
    void TearDown() override {
        belt.run(1);
        worker.run(2);
        belt.run(1);
    }
protected:
    const size_t capacity = 1;
    const size_t duration = 2;
    const size_t numArms = 2;
    ConveyorBelt belt = ConveyorBelt(1);
    ConveyorPositionController controller = ConveyorPositionController(belt,0);
    Worker worker = Worker(controller,
                    numArms,
                    {{ItemPN('A'), 1}, {ItemPN('B'),1}},
                    ItemPN('P'),
                    duration
                    );

};

// TODO: write a mock of the ConveyorPositionController to reduce testing dependency on ConveyorBelt
// TODO: write a more general test for more arms, more items, worker that only produces etc

TEST_F(WorkerTestFixture, WorkerFailTest) {
    // Throws on arms less than total quotas:
    ASSERT_THROW(Worker worker(controller,
                               1,
                               {{ItemPN('A'), 1},
                                {ItemPN('B'), 1}},
                               ItemPN('P'),
                               2
    ), invalid_argument);

    // Throws on no arms:
    ASSERT_THROW(Worker worker(controller,
                               0,
                               {{ItemPN('A'), 1},
                                {ItemPN('B'), 1}},
                               ItemPN('P'),
                               2
    ), invalid_argument);

    // This construction is valid and should not throw:
    ASSERT_NO_THROW(Worker worker(controller,
                                  2,
                                  {{ItemPN('A'), 1}, {ItemPN('B'),1}},
                                  ItemPN('P'),
                                  2
    ));

}

// Do nothing for a timeslot
TEST_F(WorkerTestFixture, WorkerDoNothingTest) {
    ASSERT_NO_THROW(worker.run(1));
}

// Put an unrelated item on the belt, worker should not take it
TEST_F(WorkerTestFixture, WorkerSkipsTest) {
    Item itm(ItemPN('C'));
    belt.enqueueItem(forward<Item>(itm));
    ASSERT_NO_THROW(worker.run(1));
    ASSERT_EQ(itm, belt.peekItem(0));

}

// Usual worker operation:
TEST_F(WorkerTestFixture, WorkerSucceedsTest) {

    // Run the belt and put a needed item on it.
    // Worker should be able to claim it:
    belt.run(1);
    belt.enqueueItem(Item(ItemPN('A')));
    ASSERT_NO_THROW(worker.run(1));
    ASSERT_TRUE(belt.isEmpty(0));

    // Run the belt and put the same item on it.
    // Worker has enough of that item and no free hands, so should not be able to claim it:
    belt.run(1);
    belt.enqueueItem(Item(ItemPN('A')));
    ASSERT_NO_THROW(worker.run(1));
    ASSERT_FALSE(belt.isEmpty(0));

    // Run the belt and put a needed item on it but reserve the timeslot.
    // Worker should not be able to claim it:
    belt.run(1);
    belt.emplaceItem(Item(ItemPN('B')),0);
    ASSERT_NO_THROW(worker.run(1));
    ASSERT_FALSE(belt.isEmpty(0));

    // Run the belt and put the other needed item on it
    // Worker should be able to claim it:
    belt.run(1);
    belt.enqueueItem(Item(ItemPN('B')));
    ASSERT_NO_THROW(worker.run(1));
    ASSERT_TRUE(belt.isEmpty(0));

    // Item should be ready within two timeslots.
    // Also check that it cannot collect an item during assembly
    belt.run(1);
    belt.enqueueItem(Item(ItemPN('A')));
    worker.run(1);
    ASSERT_FALSE(belt.isEmpty(0));
    belt.run(1);
    worker.run(1);
    ASSERT_EQ(Item(ItemPN('P')),belt.peekItem(0));
}

// Test that worker fails to place product on the belt
TEST_F(WorkerTestFixture, WorkerNoRoomTest) {
    belt.enqueueItem(Item(ItemPN('A')));
    worker.run(1);
    belt.run(1);
    belt.enqueueItem(Item(ItemPN('B')));
    worker.run(1);
    belt.run(1);
    belt.enqueueItem(Item(ItemPN('A')));
    worker.run(1);
    worker.run(1);
    ASSERT_EQ(Item(ItemPN('A')), belt.peekItem(0));
}
