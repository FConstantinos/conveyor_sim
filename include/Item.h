//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#pragma once

#include "ItemPN.h"

namespace conveyorsim {

/// This class represents items in the simulation.
///
/// Item objects, as it currently stands, only contain an ItemPN part number
/// object as its state. Future implementations could include more details
/// relevant to the simulation (like weight, quality, etc)
class Item {
public:
    /// Constructor for Item
    ///
    /// \param pn the ItemPN part number of the item
    explicit Item(const ItemPN& pn);

    /// Returns the ItemPN part number of the item
    ///
    /// \return ItemPN of the item
    [[nodiscard]] ItemPN getPN() const;

    /// Equality operator
    ///
    /// Returns true if the ItemPN part number of both objects is the same,
    /// false otherwise.
    /// \param other the item object this object compares to
    /// \return true if the objects are equal, false otherwise.
    bool operator==(const Item& other) const;

    /// Insertion operator
    ///
    /// Inserts a string representation of an Item object into an output stream.
    /// \param os the output stream the string is inserted in
    /// \param obj the Item object from which the string representation is derived
    /// \return the os stream with the string representation of obj inserted to it
    friend std::ostream& operator<<(std::ostream& os, const Item& obj);

private:
    friend std::hash<Item>;
    ItemPN pn;
};

} // conveyorsim

namespace std {

    template <>
    struct hash<conveyorsim::Item>
    {
        size_t operator()(const conveyorsim::Item& item) const noexcept
        {
            return hash<conveyorsim::ItemPN>{}(item.pn);
        }
    };
}


