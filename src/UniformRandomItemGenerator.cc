//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include <exception>
#include <ostream>
#include <random>
#include "UniformRandomItemGenerator.h"

using namespace std;
using namespace conveyorsim;

class UniformRandomItemGenerator::impl {
public:
    explicit impl(const size_t& numOutcomes) :
            rng(rd()),
            udst(0, numOutcomes - 1)
    {
        if(!numOutcomes) {
            throw invalid_argument("Attempt to construct UniformRandomItemGenerator object with no outcomes.");
        }
    }
    mutable std::random_device rd;
    mutable std::mt19937 rng;
    mutable std::uniform_int_distribution<size_t> udst;
};

UniformRandomItemGenerator::UniformRandomItemGenerator(const unordered_set<ItemPN>& PNSet, const bool& emptyPossible) :
        pImpl(make_unique<impl>(PNSet.size() + (emptyPossible ? 1 : 0))), // one more position if empty generation is possible
        PNSet(PNSet.begin(), PNSet.end())
{}

UniformRandomItemGenerator::~UniformRandomItemGenerator() = default;

vector<optional<Item>>
UniformRandomItemGenerator::get_next_items(const size_t& quantity) const
{
    vector<optional<Item>> ret;
    ret.reserve(quantity);
    for (size_t idx = 0; idx < quantity; idx++) {
        const size_t itemPNIdx = pImpl->udst(pImpl->rng);
        if (itemPNIdx == PNSet.size()) {
            ret.emplace_back(nullopt);
        } else {
            ret.emplace_back(Item(PNSet.at(itemPNIdx)));
        }
    }
    return ret;
}

optional<Item>
UniformRandomItemGenerator::get_next_item() const
{
    return get_next_items(1)[0];
}

void UniformRandomItemGenerator::print(ostream& os) const {
    os << "[ ";
    for (const auto &pn : PNSet) {
        os << pn << ", ";
    }
    os << " ]";
}
