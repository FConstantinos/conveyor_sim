//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#pragma once

#include <memory>
#include <experimental/propagate_const>
#include "SimulationComponentIF.h"

namespace conveyorsim {
/// This is a class that encapsulates the logic for running a conveyor belt simulation.
///
/// Its parameters is the conveyor capacity and the duration required for assembling an
/// item in any one of the workers. The way this configuration is set up is the following:
///  * We consider a discrete time system with timeslots as the unit of time.
///  * A UniformRandomItemGenerator is created that can generate Item objects with ItemPN
///    equal to 'A', 'B', or skip generation entirely with uniform probability for every
///    timeslot.
///  * A ConveyorBelt object is created with the given capacity and for every position on
///    the belt, two Worker objects are created with a ConveyorPositionController object
///    that they use to interact with the belt position they are assigned to.
///  * For every timeslot:
///     * an item is generated (or not) from the generator and enqueued on the first position
///       of the conveyor belt.
///     * for every position on the belt, one of the workers is given priority at random to
///       either get an Item from the position (if an item exists on the position) or to
///       deposit an Item that it had previously created on that position.
///         * the created Item has ItemPN equal to 'P' and it takes both 'A' and 'B' items
///           to be collected from the belt by the worker to be created.
///         * creation time is parameterized as described above
///         * the random priority is given to break deadlocks due to adversarial workloads
///         * if the priority worker for a given timeslot failed to either collect or
///           deposit an item, the non priority worker tries to do so instead
///         * workers cannot collect new items as long as they are busy with assembly of a
///           of a 'P' item.
///     * after the workers are done, the belt moves one position to the right, with
///       any Item that is on the rightmost position getting removed from the simulation
///     * the process begins all over again for the next timeslot
///     * statistics are collected for the number of unused 'A' and 'B' items, as well as
///       the produced 'P' items that made it through the belt.
class ABConveyorConfiguration : public SimulationComponentIF {
public:

    /// Constructor for the ABConveyorConfiguration object
    ///
    /// \param convCap capacity of the conveyor belt
    /// \param assemblyDuration duration for a single worker to construct a 'P' Item
    explicit ABConveyorConfiguration(const size_t& convCap, const size_t& assemblyDuration);

    // Defined in the implementation file, where impl is a complete type
    ~ABConveyorConfiguration();

    /// \copydoc SimulationComponentIF::run() See the class description for details.
    void run(const size_t& numSlots) override;

    /// Returns the number of 'P' items that made it through the belt
    ///
    /// \return number of 'P' items that made it through the belt by the end of the simulation run
    [[nodiscard]] size_t getProductCount() const;

    /// Returns the number of unused 'A' and 'B' items that made it through the belt
    ///
    /// \return unused 'A' and 'B' items that made it through the belt by the end of the simulation
    ///         run.
    [[nodiscard]] size_t getDropCount() const;

    /// Insertion operator
    ///
    /// Inserts a string representation of this simulation at a specific timeslot into an output
    /// stream
    /// \param os the output stream the string is inserted in
    /// \param obj the ConveyorBelt object from which the string representation is derived
    /// \return the os stream with the string representation of obj inserted to it
    friend std::ostream& operator<<(std::ostream& os, const ABConveyorConfiguration& obj);

private:
    size_t productCount;
    size_t dropCount;

    class impl;
    std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;
};

} // conveyorsim
