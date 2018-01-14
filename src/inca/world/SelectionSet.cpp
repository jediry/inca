/*
 * File: SelectionSet.cpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. Permission is granted to use and
 *      distribute this file freely for educational purposes.
 *
 * Description:
 *      This file implements the operations belonging to the SelectionSet class.
 */

// Import class definition
#include "SelectionSet.hpp"
using namespace inca;
using namespace inca::world;


/*---------------------------------------------------------------------------*
 | Element operations & tests
 *---------------------------------------------------------------------------*/
SizeType SelectionSet::size() const { return items.size(); }
bool SelectionSet::isSelected(IDType id) const {
    return (items.find(id) != items.end());
}
void SelectionSet::select(IDType id) { items.insert(id); }
void SelectionSet::deselect(IDType id) { items.erase(id); }
void SelectionSet::setSelected(IDType id, bool s) {
    s ? select(id) : deselect(id);
}


/*---------------------------------------------------------------------------*
 | Set operations & tests
 *---------------------------------------------------------------------------*/
void SelectionSet::clear() { items.clear(); }

// Mutating set operation functions
void SelectionSet::unionWith(const SelectionSet &s) {
    IDSet tmp;
    std::set_union(items.begin(), items.end(),
                   s.items.begin(), s.items.end(),
                   std::insert_iterator<IDSet>(tmp, tmp.end()));
    items.swap(tmp);
}

void SelectionSet::intersectWith(const SelectionSet &s) {
    IDSet tmp;
    std::set_intersection(items.begin(), items.end(),
                          s.items.begin(), s.items.end(),
                          std::insert_iterator<IDSet>(tmp, tmp.end()));
    items.swap(tmp);
}
void SelectionSet::differenceWith(const SelectionSet &s) {
    IDSet tmp;
    std::set_difference(items.begin(), items.end(),
                        s.items.begin(), s.items.end(),
                        std::insert_iterator<IDSet>(tmp, tmp.end()));
    items.swap(tmp);
}
void SelectionSet::symmetricDifferenceWith(const SelectionSet &s) {
    IDSet tmp;
    std::set_symmetric_difference(items.begin(), items.end(),
                                  s.items.begin(), s.items.end(),
                                  std::insert_iterator<IDSet>(tmp, tmp.end()));
    items.swap(tmp);
}
void SelectionSet::complement() {   // XXX throw unsupported
    cerr << "The set complement operation is not defined for class "
            "'inca::world::SelectionSet: please implement in your subclass";
}
void SelectionSet::selectAll() {    // XXX throw unsupported
    cerr << "The select all operation is not defined for class "
            "'inca::world::SelectionSet: please implement in your subclass";
}

// Set test functions
bool SelectionSet::isEqualTo(const SelectionSet &s) const {
    return size() == s.size() && isSupersetOf(s);
}
bool SelectionSet::isUnequalTo(const SelectionSet &s) const {
    return ! isEqualTo(s);
}
bool SelectionSet::isSupersetOf(const SelectionSet &s) const {
    return includes(items.begin(), items.end(),
                    s.items.begin(), s.items.end());
}
bool SelectionSet::isSubsetOf(const SelectionSet &s) const {
    return includes(s.items.begin(), s.items.end(),
                    items.begin(), items.end());
}
bool SelectionSet::isStrictSupersetOf(const SelectionSet &s) const {
    return size() > s.size() && isSupersetOf(s);
}
bool SelectionSet::isStrictSubsetOf(const SelectionSet &s) const {
    return size() < s.size() && isSubsetOf(s);
}
