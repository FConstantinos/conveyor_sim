//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include <ostream>
#include <random>
#include "Worker.h"
#include "UniformRandomItemGenerator.h"
#include "ConveyorBelt.h"
#include "ConveyorPositionController.h"
#include "ABConveyorConfiguration.h"

using namespace std;
using namespace conveyorsim;

class ABConveyorConfiguration::impl {
public:
    impl(const size_t& convCap, const size_t& assemblyDuration) :
            generator({ItemPN('A'), ItemPN('B')}, true),
            belt(ConveyorBelt(convCap)),
            rng(rd()),
            udst(0, 2)
    {
        for (size_t pos = 0; pos < convCap; pos++) {
            controllers.emplace_back(ConveyorPositionController(belt, pos));
        }
        for (size_t pos = 0; pos < convCap; pos++) {
            topWorkers.push_back( Worker(
                    controllers.at(pos),
                    2,
                    { {ItemPN('A'), 1}, {ItemPN('B'), 1} },
                    ItemPN('P'),
                    assemblyDuration));
            bottomWorkers.push_back( Worker(
                    controllers.at(pos),
                    2,
                    { {ItemPN('A'), 1}, {ItemPN('B'), 1} },
                    ItemPN('P'),
                    assemblyDuration));
        }
    }
    const UniformRandomItemGenerator generator;
    ConveyorBelt belt;
    vector<Worker> topWorkers;
    vector<Worker> bottomWorkers;
    vector<ConveyorPositionController> controllers;

    mutable random_device rd;
    mutable mt19937 rng;
    mutable uniform_int_distribution<size_t> udst;
};

ABConveyorConfiguration::ABConveyorConfiguration(const size_t& convCap, const size_t& assemblyDuration) :
        pImpl(make_unique<impl>(convCap, assemblyDuration)),
        productCount(0),
        dropCount(0)
{ }

ABConveyorConfiguration::~ABConveyorConfiguration() = default;

void ABConveyorConfiguration::run(const size_t& numSlots) {
    for (size_t slot = 0; slot < numSlots; slot++) {

        // Update statistics:
        const size_t& cap = pImpl->belt.getCapacity();
        const auto& peek = pImpl->belt.peekItem(cap-1);
        if (peek.has_value()) {
            if (peek.value().getPN() == ItemPN('P')) {
                productCount++;
            } else {
                dropCount++;
            }
        }

        // run the conveyor belt for one slot:
        pImpl->belt.run(1);

        // place the next item from the generator
        auto item = pImpl->generator.get_next_item();
        if (item.has_value()) {
            pImpl->belt.enqueueItem(move(item.value()));
            item = nullopt;
        }

        // Run the workers for 1 slot with random worker priority on the
        // conveyor belt position:
        const int priority = pImpl->udst(pImpl->rng);
        for(size_t pos = 0; pos < pImpl->belt.getCapacity(); pos++) {
            if (priority % 2) {
                pImpl->topWorkers.at(pos).run(1);
                pImpl->bottomWorkers.at(pos).run(1);
            } else {
                pImpl->bottomWorkers.at(pos).run(1);
                pImpl->topWorkers.at(pos).run(1);
            }
        }
    }
}

size_t ABConveyorConfiguration::getProductCount() const {
    return productCount;
}

size_t ABConveyorConfiguration::getDropCount() const {
    return dropCount;
}

namespace conveyorsim {

ostream& operator<<(ostream& os, const ABConveyorConfiguration& obj) {
    os << "***** Statistics: *****" << endl;
    os << "productCount: " << to_string(obj.productCount) << ", dropCount: " << to_string(obj.dropCount)
       << endl;
    os << "***** Conveyor Belt Status: *****" << endl;
    os << obj.pImpl->belt << endl;
    os << "***** Workers Status: *****" << endl;
    for (size_t pos = 0; pos < obj.pImpl->belt.getCapacity(); pos++) {
        os << "*** Top Worker: " << to_string(pos) << " ***" << endl;
        os << obj.pImpl->topWorkers.at(pos) << endl;
        os << "*** Bottom Worker: " << to_string(pos) << " ***" << endl;
        os << obj.pImpl->bottomWorkers.at(pos) << endl;
    }
    return os;
}

} // conveyorsim