//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#pragma once

#include <unordered_map>
#include "ConveyorPositionControllerIF.h"
#include "SimulationComponentIF.h"

namespace conveyorsim {

/// This class represents a worker on the conveyor belt. A worker is
/// presented with a conveyor positional controller which it uses to
/// manipulate the contents of the position on the conveyor belt it is
/// placed against. It collects items from the conveyor belt and produces
/// items to be placed in the conveyor belt. It is parameterized by its
/// number of arms, the needed quotas for each object before production
/// can begin, the part number of the product it produces and how long
/// it takes to assemble the object.
class Worker : public SimulationComponentIF {

public:
    /// Constructor for Worker objects
    ///
    /// \param controller interface to the position on the conveyor belt
    /// \param armsN number of arms that the Worker has to hold Item objects
    /// \param neededPNQuotas needed number of Item object with ItemPN part numbers required for product assembly
    /// \param productPN ItemPN product number of the produced Item object
    /// \param assemblyDuration duration of product assembly in timeslots
    /// \throw invalid_argument if *armsN* is 0 or is less than the total
    ///        quota of needed items
    Worker(const ConveyorPositionControllerIF& controller, const size_t& armsN,
           const std::unordered_map<ItemPN, size_t>& neededPNQuotas,
           const ItemPN& productPN, const size_t& assemblyDuration);

    /// \copydoc SimulationComponentIF::run() For every timeslot, the Worker tries to do the following
    ///          actions in order:
    ///          - try to collect an object from the belt
    ///          - try to initialize assembly of an product if the part quotas are met
    ///          - if a product is finished assembling, put it on one of its arms
    ///          - if it holds any products, try to emplace them on the conveyor belt
    void run(const size_t& numSlots) override;

    /// Insertion operator
    ///
    /// Inserts a string representation of a Worker object into an output stream
    /// \param os the output stream the string is inserted in
    /// \param obj the Worker object from which the string representation is derived
    /// \return the os stream with the string representation of obj inserted to it
    friend std::ostream& operator<<(std::ostream& os, const Worker& obj);

private:
    bool tryCollect();
    bool tryInitializeAssembly();
    bool tryFinalizeAssembly();
    bool tryReleaseProduct();
    bool canPickItem(const Item& pn) const;
    bool canUseItem(const Item& pn) const;

    const ConveyorPositionControllerIF& controller;
    const size_t assemblyDuration;
    const std::unordered_map<ItemPN, size_t> neededPNQuotas;
    const ItemPN productPN;
    const size_t armsN;

    std::unordered_map<ItemPN, size_t> heldItemCounts;
    size_t busyArms;
    size_t neededItemsCount{};
    size_t assemblyCountdown;
    bool busy;
};

} // conveyorsim
