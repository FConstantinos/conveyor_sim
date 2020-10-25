//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include "ItemPN.h"
#include <functional>

using namespace std;
using namespace conveyorsim;

ItemPN::ItemPN(const size_t &pn) : pn(pn) {}

size_t ItemPN::getPN() const
{
    return pn;
}

bool ItemPN::operator==(const ItemPN &other) const
{
    return pn == other.pn;
}

namespace conveyorsim {

ostream& operator<<(ostream& os, ItemPN const& obj)
{
    return os << to_string(obj.getPN());
}

} // conveyorsim
