//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#pragma once

#include <optional>
#include "SimulationComponentIF.h"
#include "Item.h"

namespace conveyorsim {

/// This class represents a conveyor belt that conveys items standing on its belt.
class ConveyorBeltIF {

public:
    virtual ~ConveyorBeltIF() = default;

    /// Enqueue an Item object at the start of the conveyor belt.
    ///
    /// \param item the Item object to be pushed
    virtual void enqueueItem(Item&& item) = 0;

    /// Removes and Item object from a position on the conveyor belt and returns it
    /// to the caller.
    ///
    /// \param pos position within the belt where removal takes place.
    /// \return the removed Item object
    [[nodiscard]] virtual Item collectItem(const size_t& pos) = 0;

    /// Places an Item object in a specific position on the conveyor belt.
    ///
    /// \param item the Item object to be placed.
    /// \param pos position on the conveyor belt to place the Item object.
    virtual void emplaceItem(Item&& item, const size_t& pos) = 0;

    /// Checks if a position on the conveyor belt is empty
    ///
    /// \param pos the checked position on the conveyor belt
    /// \return true if the position is empty, false otherwise
    [[nodiscard]] virtual bool isEmpty(const size_t& pos) const = 0;

    /// Checks if a position on the conveyor belt is reserved
    /// during the timeslot and therefore not amenable to
    /// changes
    ///
    /// \param pos the checked position on the conveyor belt
    /// \return true if the position is reserved, false otherwise
    [[nodiscard]] virtual bool isReserved(const size_t& pos) const = 0;

    /// Returns a constant reference to an Item object at a position on the
    /// conveyor belt.
    ///
    /// \param pos the position of the Item object
    /// \return a constant reference to the Item object in pos, nullopt if the
    /// position is empty.
    [[nodiscard]] virtual const std::optional<Item>& peekItem(const size_t& pos) const = 0;

    /// Returns the capacity of the conveyor belt.
    ///
    /// \return capacity of the conveyor belt
    [[nodiscard]] virtual size_t getCapacity() const = 0;

    /// Insertion operator
    ///
    /// Inserts a string representation of a conveyor belt into an output stream
    /// \param os the output stream the string is inserted in
    /// \param obj the conveyor belt object from which the string representation is derived
    /// \return the os stream with the string representation of obj inserted to it
    friend std::ostream& operator<<(std::ostream& os, const ConveyorBeltIF& obj);

private:
    virtual void print(std::ostream&) const = 0;

};

} // conveyorsim
