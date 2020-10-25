//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include <gtest/gtest.h>
#include "ConveyorBelt.h"

using namespace std;
using namespace conveyorsim;

struct ConveyorBeltTestCase {
    const size_t capacity;
};

class ConveyorBeltTestFixture : public ::testing::TestWithParam<ConveyorBeltTestCase> {};

// TODO: write a test to run for multiple slots, or even 0 slots
TEST_P(ConveyorBeltTestFixture, ConveyorBeltTest) {
    const auto testCase = GetParam();
    const auto &cap = testCase.capacity;

    if(!cap) {
        ASSERT_THROW(ConveyorBelt belt(cap), invalid_argument);
        return;
    }

    ASSERT_NO_THROW(ConveyorBelt belt(cap));
    ConveyorBelt belt(cap);

    // Test emplacing items over the length of the belt
    ASSERT_EQ(cap, belt.getCapacity());
    for(size_t pos = 0; pos < cap; pos++) {
        ASSERT_TRUE(belt.isEmpty(pos));
        ASSERT_FALSE(belt.isReserved(pos));
        Item itm = Item(ItemPN(pos));
        ASSERT_NO_THROW(belt.emplaceItem(forward<Item>(itm), pos));
        ASSERT_FALSE(belt.isEmpty(pos));
        ASSERT_TRUE(belt.isReserved(pos));
        ASSERT_EQ(itm, belt.peekItem(pos));
        ASSERT_THROW(belt.emplaceItem(forward<Item>(itm), pos), invalid_argument);
        // cast to void is required to suppress compiler warning
        // because collectItem has the nodiscard attribute
        ASSERT_THROW(static_cast<void>(belt.collectItem(pos)), invalid_argument);
    }

    // Run the belt one timeslot, test that the items are in the correct positions and test
    // collecting all of the items:
    ASSERT_NO_THROW(belt.run(1));
    for(size_t pos = 1; pos < cap; pos++) {
        ASSERT_FALSE(belt.isEmpty(pos));
        ASSERT_FALSE(belt.isReserved(pos));
        Item itm = Item(ItemPN(pos-1));
        ASSERT_NO_THROW(static_cast<void>(belt.collectItem(pos)));
        ASSERT_TRUE(belt.isEmpty(pos));
        ASSERT_TRUE(belt.isReserved(pos));
        ASSERT_EQ(nullopt, belt.peekItem(pos));
        ASSERT_THROW(belt.emplaceItem(forward<Item>(itm), pos), invalid_argument);
        ASSERT_THROW(static_cast<void>(belt.collectItem(pos)), invalid_argument);
    }
    // Test enqueuing an item (enqueuing is instantaneous and should not leave the 0 position
    // reserved
    Item itm = Item(ItemPN(0));
    ASSERT_NO_THROW(belt.enqueueItem(forward<Item>(itm)));
    ASSERT_FALSE(belt.isEmpty(0));
    ASSERT_FALSE(belt.isReserved(0));
    ASSERT_EQ(itm, belt.peekItem(0));
}

vector<ConveyorBeltTestCase> belttc = {
        {0},
        {1},
        {10},
        {100},
        {1000},
};

INSTANTIATE_TEST_CASE_P(
        ConveyorBeltTest,
        ConveyorBeltTestFixture,
        ::testing::ValuesIn(belttc)
);
