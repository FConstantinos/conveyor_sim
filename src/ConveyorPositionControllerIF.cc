//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include "ConveyorPositionControllerIF.h"

using namespace std;
using namespace conveyorsim;

namespace conveyorsim {

ostream& operator<<(ostream& os, const ConveyorPositionControllerIF& obj) {
    obj.print(os);
    return os;
}

} // conveyorsim