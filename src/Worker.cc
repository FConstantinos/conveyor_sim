//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include <exception>
#include <ostream>
#include "Worker.h"

using namespace std;
using namespace conveyorsim;

Worker::Worker(const ConveyorPositionControllerIF& controller, const size_t& armsN,
               const unordered_map<ItemPN, size_t>& neededPNQuotas,
               const ItemPN& productPN, const size_t& assemblyDuration) :
        controller(controller),
        armsN(armsN),
        neededPNQuotas(neededPNQuotas),
        productPN(productPN),
        assemblyDuration(assemblyDuration),
        assemblyCountdown(0),
        busyArms(0),
        busy(false)
{
    if (!armsN) {
        throw invalid_argument(string(__func__) + ": Attempt to construct a worker object with no arms");
    }
    for(const auto &[pn, quota]: neededPNQuotas) {
        neededItemsCount += quota;
        heldItemCounts[pn] = 0;
    }
    heldItemCounts[productPN] = 0;

    if(armsN < neededItemsCount) {
        throw invalid_argument(string(__func__) + ": Attempt to construct a worker object with less arms than the "
                                                  "number of needed items");
    }
}

bool
Worker::tryCollect()
{
    const auto& peek = controller.peekItem();
    if(!peek.has_value()) {
        return false;
    }
    if( !(canPickItem(peek.value()) && canUseItem(peek.value())) ) {
        return false;
    }

    const auto item = controller.collectItem();
    const auto& pn = item.getPN();
    heldItemCounts.at(pn)++;
    neededItemsCount--;
    busyArms++;

    // TODO: actual item will be destroyed here since its only relevant state (pn) is now stored.
    //       In a future implementation where more state is added, we may need a DS to store the
    //       items held by the worker (e.g weight of the object, quality, serial numbers etc)
    return true;
}

bool Worker::tryInitializeAssembly()
{
    // If you are still busy with a current assembly, do nothing:
    if (busy) {
        return false;
    }

    // If you don't have enough items, do nothing:
    if (neededItemsCount) {
        return false;
    }

    busy = true;
    assemblyCountdown = assemblyDuration;
    return true;
}

bool
Worker::tryFinalizeAssembly()
{
    // If you are still busy with a current assembly, or haven't started yet, do nothing:
    if ( !(busy && !assemblyCountdown)) {
        return false;
    }
    busy = false;
    for(const auto& [key, quota]: neededPNQuotas) {
        size_t& count = heldItemCounts.at(key);
        count -= quota;
        neededItemsCount += (quota >= count) ? quota - count : 0;
        busyArms -= quota;
    }
    heldItemCounts.at(productPN)++;
    busyArms++;
    return true;
}

bool
Worker::tryReleaseProduct()
{
    if (!(!controller.isReserved() && controller.isEmpty() && heldItemCounts.at(productPN))) {
        return false;
    }
    heldItemCounts.at(productPN)--;
    busyArms--;
    controller.emplaceItem(Item(productPN));
    return true;
}

bool
Worker::canUseItem(const Item& item) const
{
    const ItemPN& pn = item.getPN();

    // Item not needed:
    if (!neededPNQuotas.count(pn)) {
        return false;
    }

    // Missing needed item:
    if(heldItemCounts.at(pn) < neededPNQuotas.at(pn)) {
        return true;
    } else {

        // Surplus needed item. Can be used only if there is room for the remaining
        // non surplus items that are needed. This is to prevent deadlocks.
        if ( (armsN - busyArms) <= neededItemsCount) {
            return false;
        }
        return true;
    }
}

bool
Worker::canPickItem(const Item& item) const
{
    return !controller.isReserved() && !busy && (busyArms < armsN);
}

void Worker::run(const size_t &numSlots) {
    for(size_t slot = 0; slot < numSlots; slot++) {

        if (assemblyCountdown) {
            assemblyCountdown--;
        }
        tryCollect();
        tryInitializeAssembly();
        tryFinalizeAssembly();
        tryReleaseProduct();
    }
}

namespace conveyorsim {

ostream& operator<<(ostream& os, const Worker& obj) {

    os << "[ ";
    os << obj.productPN << ", ";
    os << "numProducts : " << obj.heldItemCounts.at(obj.productPN) << ", ";
    os << "controller : " << obj.controller << ", ";
    os << "heldItemCounts : { ";
    for (const auto& [pn, quota]: obj.neededPNQuotas) {
        os << "{ pn : " << pn << ", ";
        os << "count : " << obj.heldItemCounts.at(pn) << ", ";
        os << "quota : " << quota << "}, ";
    }
    os << " }, ";
    os << "armsN : " << to_string(obj.armsN) << ", ";
    os << "busyArms : " << to_string(obj.busyArms) << ", ";
    os << "neededItemsCount : " << to_string(obj.neededItemsCount) << ", ";
    os << "assemblyDuration : " << to_string(obj.assemblyDuration) << ", ";
    os << "assemblyCountdown : " << to_string(obj.assemblyCountdown) << ", ";
    os << "busy : " << boolalpha << obj.busy << noboolalpha << " ";
    os << "]";

    return os;
}

} // conveyorsim
