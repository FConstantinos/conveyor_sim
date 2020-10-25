//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#pragma once

#include <optional>
#include <unordered_set>
#include <experimental/propagate_const>
#include <memory>

#include "ItemGeneratorIF.h"
#include "Item.h"

namespace conveyorsim {

/// This class represents an Item generator that creates items in a range of ItemPN with
/// uniform random probability.
class UniformRandomItemGenerator : ItemGeneratorIF {
public:

    /// Constructor for UniformRandomItemGenerator
    ///
    /// It configures the generator object to produce Item within a range of ItemPN part numbers.
    /// It can also configure the generator to not produce an item as one of the uniform random choices.
    /// \param PNSet set of possible ItemPN for each generated item
    /// \param emptyPossible makes it possible for the generator to not produce an item.
    /// \throws invalid_argument if there are no possible outcomes (PNSet is empty and emptyPossible is false)
    explicit UniformRandomItemGenerator(const std::unordered_set<ItemPN>& PNSet, const bool& emptyPossible=false);

    // Defined in the implementation file, where impl is a complete type
    ~UniformRandomItemGenerator();

    /// Returns a vector with the next Item objects produced by the generator object.
    ///
    /// \param quantity number of trials
    /// \return a vector of size equal to the number of trials with the produced Item objects
    ///         of the generator.
    [[nodiscard]] std::vector<std::optional<Item>> get_next_items(const size_t &quantity) const override;

    /// Returns the next Item object produced by the generator object.
    ///
    /// \return next Item object produced by the generator (or nullopt if failing to
    ///         create an Item object is enabled)
    [[nodiscard]] std::optional<Item> get_next_item() const override;

private:
    void print(std::ostream& os) const override;

    const std::vector<ItemPN> PNSet;

    class impl;
    std::unique_ptr<impl> pImpl;
};

} // conveyorsim
