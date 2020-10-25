#---------------------------------------------------------------------------------------------------
# Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
#---------------------------------------------------------------------------------------------------

cmake_minimum_required(VERSION 3.16)
include(GNUInstallDirs)

project(conveyor_sim)

set(CMAKE_CXX_STANDARD 17)
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

########################################################################
## Targets
########################################################################
find_package(Boost 1.71.0 REQUIRED)

add_executable(conveyor_sim
        src/conveyor_sim.cc
        src/ABConveyorConfiguration.cc
        src/ConveyorBelt.cc
        src/Worker.cc
        src/ConveyorPositionControllerIF.cc
        src/ConveyorPositionController.cc
        src/ConveyorBeltIF.cc src/ItemGeneratorIF.cc
        src/UniformRandomItemGenerator.cc
        src/Item.cc
        src/ItemPN.cc
        unittests/UniformRandomItemGenerator_tests.h)

include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
        ${Boost_INCLUDE_DIRS}
)

install(TARGETS conveyor_sim
        RUNTIME
        DESTINATION ${CMAKE_INSTALL_BINDIR}
        )

set_target_properties(conveyor_sim PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )

########################################################################
## Sub-directory Targets
########################################################################
option(ENABLE_TEST "Build tests" OFF)

if (ENABLE_TEST)
    add_subdirectory(unittests)
endif(ENABLE_TEST)

########################################################################
## Documentation
########################################################################
find_package(Doxygen REQUIRED dot)
option(BUILD_DOCUMENTATION  "Create and install the HTML based documentation (requires Doxygen)" OFF)
if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    endif()

    set(doxyfile_in ${CMAKE_CURRENT_SOURCE_DIR}/doc/conveyor_sim.doxygen)
    set(doxyfile ${CMAKE_CURRENT_BINARY_DIR}/doxyfile)

    configure_file(${doxyfile_in} ${doxyfile} @ONLY)

    message("Doxygen build started.")

    add_custom_target(doc ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile_in}
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
endif()
