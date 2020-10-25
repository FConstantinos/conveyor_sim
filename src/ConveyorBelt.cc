//
// Copyright (c) 2020 Konstantinos Fragkiadakis. All rights reserved.
//

#include <exception>
#include <string>
#include <boost/circular_buffer.hpp>
#include "ConveyorBelt.h"

using namespace std;
using namespace conveyorsim;

class ConveyorBelt::impl {
public:
    explicit impl(const size_t& capacity) : belt(capacity, nullopt) {}
    boost::circular_buffer<std::optional<Item>> belt;
};

namespace {
    string outOfRangeErr(const string &methodName, const size_t &pos, const size_t &cap) {
        return methodName + ": pos argument is greater or equal to conveyor belt capacity: pos = "
                            + to_string(pos) + " belt capacity = " +  to_string(cap);
    }

    string emptyPosErr(const string &methodName, const size_t &pos) {
        return methodName + ": nothing in pos: pos = " + to_string(pos);
    }

    string nonEmptyPosErr(const string &methodName, const size_t &pos) {
        return methodName + ": pos is occupied by another object: pos = " + to_string(pos);
    }

    string reservedErr(const string &methodName, const size_t &pos) {
        return methodName + ": pos is reserved: pos = " + to_string(pos);
    }
}

ConveyorBelt::ConveyorBelt(const size_t &capacity) :
pImpl(make_unique<impl>(capacity)),
reserved(capacity,false)
{
    if (!capacity) {
        throw invalid_argument(string(__func__) + ": attempt to construct a zero capacity conveyor belt");
    }
    for (size_t pos = 0; pos < capacity; pos++) {
        pImpl->belt[pos] = nullopt;
    }
}

ConveyorBelt::~ConveyorBelt() = default;
ConveyorBelt::ConveyorBelt(ConveyorBelt&& ) noexcept = default;

void
ConveyorBelt::enqueueItem(Item&& item)
{
    if (pImpl->belt.at(0) != nullopt) {
        throw invalid_argument(nonEmptyPosErr(__func__, 0));
    }
    if (isReserved(0)) {
        throw runtime_error(reservedErr(__func__, 0));
    }
    pImpl->belt.at(0).emplace(move(item));
}

Item
ConveyorBelt::collectItem(const size_t &pos)
{
    if (!validPos(pos)) {
        throw out_of_range(outOfRangeErr(__func__, pos, pImpl->belt.capacity()));
    }
    if(pImpl->belt.at(pos) == nullopt) {
        throw invalid_argument(emptyPosErr(__func__, pos));
    }
    if (isReserved(pos)) {
        throw invalid_argument(reservedErr(__func__, pos));
    }
    Item it = pImpl->belt.at(pos).value();
    pImpl->belt.at(pos) = nullopt;
    reserved.at(pos) = true;
    return it;
}

void ConveyorBelt::emplaceItem(Item&& item, const size_t& pos) {
    if (!validPos(pos)) {
        throw out_of_range(outOfRangeErr(__func__, pos, pImpl->belt.capacity()));
    }
    if (isReserved(pos)) {
        throw invalid_argument(reservedErr(__func__, pos));
    }
    if (pImpl->belt.at(pos) != nullopt) {
        throw invalid_argument(nonEmptyPosErr(__func__, pos));
    }
    pImpl->belt.at(pos).emplace(move(item));
    reserved.at(pos) = true;
}

bool
ConveyorBelt::isEmpty(const size_t& pos) const
{
    if (!validPos(pos)) {
        throw out_of_range(outOfRangeErr(__func__, pos, pImpl->belt.capacity()));
    }
    return peekItem(pos) == nullopt;
}

bool
ConveyorBelt::isReserved(const size_t& pos) const {
    if (!validPos(pos)) {
        throw out_of_range(outOfRangeErr(__func__, pos, pImpl->belt.capacity()));
    }
    return reserved.at(pos);
}

const optional<Item>&
ConveyorBelt::peekItem(const size_t &pos) const
{
    if (!validPos(pos)) {
        throw out_of_range(outOfRangeErr(__func__, pos, pImpl->belt.capacity()));
    }
    return pImpl->belt.at(pos);
}

void
ConveyorBelt::rotate()
{
    pImpl->belt.push_front(nullopt);
}

bool
ConveyorBelt::validPos(const size_t &pos) const
{
    return pos < pImpl->belt.capacity();
}

size_t ConveyorBelt::getCapacity() const {
    return pImpl->belt.capacity();
}

void ConveyorBelt::run(const size_t &numSlots) {
    for(size_t i = 0; i < numSlots; i++) {
        rotate();
    }
    for (size_t pos = 0; pos < getCapacity(); pos++) {
        reserved.at(pos) = false;
    }
}

void ConveyorBelt::print(ostream& os) const {
    os << "[ ";
    for(size_t pos = 0; pos < getCapacity(); pos++) {
        os << "{ ";
        os << to_string(pos) << ": ";
        const auto &peek = peekItem(pos);
        if(peek.has_value()) {
            os << peek.value().getPN();
        } else {
            os << "empty";
        }
        os << ", reserved: " << boolalpha << isReserved(pos) << noboolalpha;
        os << " }";
        if ( pos < getCapacity()-1) {
            os << ", ";
        }
    }
    os << " ]";
}
