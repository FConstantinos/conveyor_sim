//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#pragma once

#include <cstddef>

namespace conveyorsim {

/// This is an abstract class that represents any simulation component
/// in a discrete time simulation.
class SimulationComponentIF {
public:
    virtual ~SimulationComponentIF() = default;

    /// Runs the simulation component
    ///
    /// \param numSlots number of timeslots to run the simulation.
    virtual void run(const size_t& numSlots) = 0;
};

} // conveyorsim
