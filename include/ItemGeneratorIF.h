//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#pragma once

#include <optional>
#include "Item.h"

namespace conveyorsim {

/// This class represents an Item generator that creates Item objects.
class ItemGeneratorIF {
public:

    ~ItemGeneratorIF() = default;

    /// Returns a vector with the next Item objects produced by the generator object.
    ///
    /// \param trials number of trials
    /// \return a vector of size equal to the number of trials with the produced Item objects
    ///         of the generator.
    [[nodiscard]] virtual std::vector<std::optional<Item>> get_next_items(const size_t &trials) const = 0;

    /// Returns the next Item object produced by the generator object.
    ///
    /// \return next Item object produced by the generator (or nullopt if failing to
    ///         create an Item object is enabled)
    [[nodiscard]] virtual std::optional<Item> get_next_item() const = 0;
    friend std::ostream& operator<<(std::ostream& os, const ItemGeneratorIF& obj);

private:
    virtual void print(std::ostream& os) const = 0;
};

} // conveyorsim
