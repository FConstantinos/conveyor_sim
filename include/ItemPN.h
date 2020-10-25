//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#pragma once

#include <cstddef>
#include <functional>

namespace conveyorsim {

/// This class represents part numbers of items in the simulation.
///
/// Part numbers uniquely identify an item *design* as opposed to its instantiation
/// (which would be identified by a serial number instead).
/// Part numbers, as it currently stands, are simply represented by a
/// size_t integer number.
class ItemPN {
public:

    /// Constructor for ItemPN
    ///
    /// \param pn the part number representation of an item
    explicit ItemPN(const size_t& pn);

    /// Return the part number as a size_t representation
    ///
    /// \return size_t representation of a part number
    [[nodiscard]] size_t getPN() const;

    /// Equality operator
    ///
    /// Returns true if the size_t part number representation of both objects is the same,
    /// false otherwise.
    /// \param other the part number object this object compares to
    /// \return true if the objects are equal, false otherwise.
    bool operator==(const ItemPN &other) const;

    /// Insertion operator
    ///
    /// Inserts a string representation of a part number object into an output stream
    /// \param os the output stream the string is inserted in
    /// \param obj the part number object from which the string representation is derived
    /// \return the os stream with the string representation of obj inserted to it
    friend std::ostream& operator<<(std::ostream& os, const ItemPN& obj);

private:
    friend std::hash<ItemPN>;
    size_t pn;
};

} // conveyorsim

namespace std {

    template <>
    struct hash<conveyorsim::ItemPN>
    {
        size_t operator()(const conveyorsim::ItemPN& itemPn) const noexcept
        {
            return hash<size_t>{}(itemPn.pn);
        }
    };
}
