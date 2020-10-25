//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include <exception>
#include <ostream>
#include "ConveyorPositionController.h"

using namespace std;
using namespace conveyorsim;

ConveyorPositionController::ConveyorPositionController(ConveyorBeltIF &belt, const size_t& pos) :
belt(belt),
pos(pos)
{
    if (belt.getCapacity() <= pos) {
        throw out_of_range(string(__func__) + ": attempt to construct instance of ConveyorPositionController with "
                                                  "out of bounds pos argument");
    }
}

Item ConveyorPositionController::collectItem() const {
    return belt.collectItem(pos);
}

void ConveyorPositionController::emplaceItem(Item &&item) const {
    belt.emplaceItem(move(item), pos);
}

bool ConveyorPositionController::isEmpty() const {
    return belt.isEmpty(pos);
}

bool ConveyorPositionController::isReserved() const {
    return belt.isReserved(pos);
}

const optional<Item> &ConveyorPositionController::peekItem() const {
    return belt.peekItem(pos);
}

void ConveyorPositionController::print(ostream& os) const {
    const auto& peek = peekItem();
    os << "[ ";
    if (peek.has_value()) {
        os << peek.value().getPN();
    } else {
        os << "empty";
    }
    os << ", reserved: " << boolalpha << isReserved() << noboolalpha << " ]";
}
