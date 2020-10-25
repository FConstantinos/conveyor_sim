//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#pragma once

#include "ConveyorBeltIF.h"
#include "SimulationComponentIF.h"
#include <experimental/propagate_const>
#include <memory>

namespace conveyorsim {

/// This class represents a conveyor belt that conveys items standing on its belt.
///
/// The conveyor belt is defined by a capacity, the number of positions that
/// are available to it that can hold conveyed items. It moves from left to right.
/// At each unit of time and for every position, an item can be either placed on the
/// position or removed from the position, but not both at the same time.
/// It is implemented with a circular buffer of constant capacity. This is because all
/// useful operations (add/remove/access an element as well as rotate the data
/// structure) are done in O(1) time complexity and O(N) space complexity, where
/// N is the capacity of the conveyor belt.
class ConveyorBelt : public ConveyorBeltIF, public SimulationComponentIF {
public:

    /// Constructor for a ConveyorBelt object.
    ///
    /// \param capacity the capacity of the conveyor belt in number of Item object positions.
    /// \throws invalid_argument when *capacity* is 0
    explicit ConveyorBelt(const size_t& capacity);

    // Defined in the implementation file, where impl is a complete type
    ~ConveyorBelt() override;
    ConveyorBelt(ConveyorBelt&& ) noexcept;

    /// \copydoc ConveyorBeltIF::enqueueItem
    /// \throws invalid_argument if the first position on the conveyor belt
    ///         is non-empty
    /// \throws runtime_error if the first position on the conveyor belt
    ///         is reserved
    void enqueueItem(Item&& item) override;

    /// \copydoc ConveyorBeltIF::collectItem
    /// \throws out_of_range if the *pos* argument indicates a position
    ///         beyond the capacity of the conveyor belt
    /// \throws invalid_argument if the *pos* argument indicates a position
    ///         on the conveyor belt that is empty or reserved.
    [[nodiscard]] Item collectItem(const size_t& pos) override;

    /// \copydoc ConveyorBeltIF::emplaceItem
    /// \throws out_of_range if the *pos* argument indicates a position
    ///         beyond the capacity of the conveyor belt
    /// \throws invalid_argument if the *pos* argument indicates a position
    ///         on the conveyor belt that is non-empty or reserved.
    void emplaceItem(Item&& item, const size_t& pos) override;

    /// \copydoc ConveyorBeltIF::isEmpty
    /// \throws out_of_range if the *pos* argument indicates a position
    ///         beyond the capacity of the conveyor belt
    [[nodiscard]] bool isEmpty(const size_t& pos) const override;

    /// \copydoc ConveyorBeltIF::isReserved
    /// \throws out_of_range if the *pos* argument indicates a position
    ///         beyond the capacity of the conveyor belt
    [[nodiscard]] bool isReserved(const size_t& pos) const override;

    /// \copydoc ConveyorBeltIF::peekItem
    /// \throws out_of_range if the *pos* argument indicates a position
    ///         beyond the capacity of the conveyor belt
    [[nodiscard]] const std::optional<Item>& peekItem(const size_t& pos) const override;

    /// \copydoc ConveyorBeltIF::getCapacity
    [[nodiscard]] size_t getCapacity() const override;

    /// \copydoc SimulationComponentIF::run See class description for details.
    void run(const size_t& numSlots) override;

private:
    /// Returns true if the *pos* argument represents a position on the capacity
    /// of the conveyor belt, false otherwise
    ///
    /// \param pos position on the conveyor belt
    /// \return true if *pos* represents a valid position on
    ///         the conveyor belt, false otherwise.
    [[nodiscard]] bool validPos(const size_t& pos) const;

    /// Rotates the conveyor belt one step to the left. If an Item object on the last
    /// position of the belt is present, that item is destroyed.
    void rotate();
    void print(std::ostream& os) const override;

    std::vector<bool> reserved;

    class impl;
    std::experimental::propagate_const<std::unique_ptr<impl>> pImpl;
};

} // conveyorsim
