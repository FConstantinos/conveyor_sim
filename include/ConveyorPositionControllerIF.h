//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#pragma once

#include <optional>
#include "Item.h"

namespace conveyorsim {

/// Abstract Interface that controls the interaction between a Worker object
/// and the ConveyorBelt object.
///
/// It maps to a position within the conveyor
/// belt and is used by the worker to manipulate that position (place, collect
/// peek items)
class ConveyorPositionControllerIF {
public:
    virtual ~ConveyorPositionControllerIF() = default;;

    /// \copybrief ConveyorBeltIF::collectItem
    ///
    /// \return the removed Item object
    [[nodiscard]] virtual Item collectItem() const = 0;

    /// \copybrief ConveyorBeltIF::emplaceItem
    ///
    /// \param item the Item object to be placed
    virtual void emplaceItem(Item&& item) const = 0;

    /// \copybrief ConveyorBeltIF::collectItem
    ///
    /// \return true if the position is empty, false otherwise
    [[nodiscard]] virtual bool isEmpty() const = 0;

    /// \copybrief ConveyorBeltIF::collectItem
    ///
    /// \return true if the position is reserved, false otherwise
    [[nodiscard]] virtual bool isReserved() const = 0;

    /// \copybrief ConveyorBeltIF::collectItem
    ///
    /// \return a constant reference to the Item object in pos, nullopt if the
    ///         position is empty.
    [[nodiscard]] virtual const std::optional<Item>& peekItem() const = 0;

    /// Insertion operator
    ///
    /// Inserts a string representation of a conveyor position controller into an output stream
    /// \param os the output stream the string is inserted in
    /// \param obj the conveyor position controller object from which the string representation is derived
    /// \return the os stream with the string representation of obj inserted to it
    friend std::ostream& operator<<(std::ostream& os, const ConveyorPositionControllerIF& obj);

private:
    virtual void print(std::ostream&) const = 0;
};

} // conveyorsim
