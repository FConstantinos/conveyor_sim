//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include "ConveyorBeltIF.h"
using namespace conveyorsim;
using namespace std;

namespace conveyorsim {
ostream& operator<<(ostream& os, const ConveyorBeltIF& obj) {
    obj.print(os);
    return os;
}
}