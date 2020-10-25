#---------------------------------------------------------------------------------------------------
# Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
#---------------------------------------------------------------------------------------------------

cmake_minimum_required (VERSION 3.16)

# The Google Test framework can generate a header and source code file
# that can be included in the repository. An advatnage of that is that
# it avoids having GTest as a requirement. A disadvantage of that is
# that the developers of this repository now need to make sure GTest is
# up to date. Another small disadvantage is the initial build time; GTest
# will also have to build. Subsequent builds however should cache the
# GTest build.
#
# For more information, see here:
# https://github.com/google/googletest/tree/master/googletest#incorporating-into-an-existing-cmake-project
#
# Here we elect to require the package:
find_package(GTest REQUIRED)

########################################################################
## Targets
########################################################################
add_executable(conveyor_sim_test
               conveyor_sim_test.cc
               ../src/Worker.cc
               ../src/ConveyorPositionControllerIF.cc
               ../src/ConveyorPositionController.cc
               ../src/ConveyorBelt.cc
               ../src/ItemGeneratorIF.cc
               ../src/UniformRandomItemGenerator.cc
               ../src/Item.cc
               ../src/ItemPN.cc
        )

include_directories(
        ${PROJECT_SOURCE_DIR}/unittests
)

target_link_libraries(conveyor_sim_test
        GTest::GTest
        )