//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#pragma once

#include "ConveyorBeltIF.h"
#include "ConveyorPositionControllerIF.h"

namespace conveyorsim {

class ConveyorPositionController : public ConveyorPositionControllerIF {
public:
    /// Constructor for ConveyorPositionController objects
    ///
    /// \param belt the underlying conveyor belt
    /// \param pos the underlying position on the conveyor belt
    ///        controlled by the position controller
    /// \throws out_of_range if the underlying position is beyond the
    ///         capacity of the conveyor belt.
    ConveyorPositionController(ConveyorBeltIF& belt, const size_t& pos);

    /// \copydoc ConveyorPositionControllerIF::collectItem()
    [[nodiscard]] Item collectItem() const override;

    /// \copydoc ConveyorPositionControllerIF::emplaceItem()
    void emplaceItem(Item&& item) const override;

    /// \copydoc ConveyorPositionControllerIF::isEmpty()
    [[nodiscard]] bool isEmpty() const override;

    /// \copydoc ConveyorPositionControllerIF::isReserved()
    [[nodiscard]] bool isReserved() const override;

    /// \copydoc ConveyorPositionControllerIF::peekItem()
    [[nodiscard]] const std::optional<Item>& peekItem() const override;

private:
    void print(std::ostream& os) const override;
    const size_t pos;
    ConveyorBeltIF& belt;
};

} // conveyorsim
