//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include <ostream>
#include "Item.h"

using namespace std;
using namespace conveyorsim;

Item::Item(const ItemPN& pn) : pn(pn) {}

ItemPN Item::getPN() const
{
    return pn;
}

bool Item::operator==(const Item& other) const
{
    return (pn == other.pn);
}

namespace conveyorsim {

ostream& operator<<(ostream& os, const Item& obj) {
    return os << "[ PN:" << obj.pn << "]";
}

} // conveyorsim

