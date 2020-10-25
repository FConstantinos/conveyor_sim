//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include <gtest/gtest.h>

#include "UniformRandomItemGenerator_tests.h"
#include "ConveyorBelt_tests.h"
#include "Worker_tests.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}