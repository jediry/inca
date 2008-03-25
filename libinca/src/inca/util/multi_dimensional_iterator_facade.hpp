/* -*- C++ -*-
 *
 * File: multi_dimensional_iterator_facade.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. All rights reserved.
 *
 * Description:
 *      The MultiDimensionalIterator template class implements an iterator
 *      with semantics defined for multiple dimensions. Its most obvious use
 *      is as the iterator for the MultiArray generic container, but it could
 *      be specialized to work for any multi-dimensional purpose.
 *
 *      XXX old docs begin here
 *      The MultiArrayIterator class allows to iterate through every element
 *      in the MultiArray in a linear fashion just like any iterator,
 *      regardless of the dimensionality of the container. Additionally, it
 *      offers a multi-dimensional interface, allowing to move freely
 *      through the container, along any axis.
 *
 *      The linear iteration functionality is accessed as with any
 *      random-access iterator:
 *          i++, i--    increment, decrement
 *          i += n      jump
 *
 *      The multi-dimensional iteration functionality is accessed analogously,
 *      but through an array of AxisWalker objects, which implement the
 *      requisite iterator operators (minus the accessors * and ->) by
 *      stepping the parent iterator by the appropriate number of elements:
 *          i[0]++      increment along the first axis
 *          i[n] += m   jump along the nth axis
 *
 *      The n-dimensional coordinates
 */

// Questions:
//      Conflict between using [] for axes and traditional ptr-style meaning
//      Good way to do n-D coordinates
// TODO:


#pragma once
#ifndef INCA_UTIL_MULTI_DIMENSIONAL_ITERATOR_FACADE_HPP
#define INCA_UTIL_MULTI_DIMENSIONAL_ITERATOR_FACADE_HPP

// Import superclass definition
#include <boost/iterator/iterator_facade.hpp>

// This is part of the Inca utilities collection
namespace inca {
    // Forward declarations
    class multi_dimensional_iterator_core_access;
    template <class Iterator> class axis_iterator_facade;
    template <class Derived,
              class Value,
              class CategoryOrTraversal,
              class Reference = Value &,
              class Difference = ptrdiff_t,
              class Index = Difference>
        class multi_dimensional_iterator_facade;
};


// Derivations of multi_dimensional_iterator_facade need only grant
// friendship to this class
class inca::multi_dimensional_iterator_core_access : boost::iterator_core_access {
    // The facade classes need friendship so they can call the Derived class's functions
    template <class Dv, class V, class TC, class R, class D, class I>
        friend class multi_dimensional_iterator_facade;
    template <class I> friend class axis_iterator_facade;

    template <class Facade>
    static typename Facade::index_type index(Facade &f, typename Facade::index_type d)
    {
        return f.index(d);
    }

    template <class Facade>
    static void increment(Facade &f, typename Facade::index_type d)
    {
        f.increment(d);
    }

    template <class Facade>
    static void decrement(Facade &f, typename Facade::index_type d)
    {
        f.decrement(d);
    }

    template <class Facade>
    static void advance(Facade &f, typename Facade::index_type d,
                                   typename Facade::difference_type n)
    {
        f.advance(d, n);
    }

private:
    // objects of this class are useless
    multi_dimensional_iterator_core_access(); //undefined
};


// Facade class for stepping the owning multi dimensional iterator by some
// number of elements along a single dimensional axis. An instance of this
// is returned whenever a single axis of the iterator is accessed.
template <class Iterator>
class inca::axis_iterator_facade {
public:
    typedef typename Iterator::difference_type  difference_type;
    typedef typename Iterator::index_type       index_type;

public:
    // Constructor
    explicit axis_iterator_facade(index_type dim, Iterator & it)
        : _dimension(dim), _iterator(it) { }

    // Accessor functions
    index_type dimension() const { return _dimension; }
    Iterator & iterator()  const { return _iterator; }

protected:
    index_type _dimension;
    Iterator & _iterator;

public:
    // Operator implementations
    operator index_type() const
    {
        return multi_dimensional_iterator_core_access::index(_iterator,
                                                             _dimension);
    }

    Iterator& operator++()
    {
        multi_dimensional_iterator_core_access::increment(_iterator,
                                                          _dimension);
        return _iterator;
    }

    Iterator operator++(int)
    {
        Iterator tmp(_iterator);
        ++*this;
        return tmp;
    }

    Iterator& operator--()
    {
        multi_dimensional_iterator_core_access::decrement(_iterator,
                                                          _dimension);
        return _iterator;
    }

    Iterator operator--(int)
    {
        Iterator tmp(_iterator);
        --*this;
        return tmp;
    }

    Iterator& operator+=(difference_type n)
    {
        multi_dimensional_iterator_core_access::advance(_iterator,
                                                        _dimension, n);
        return _iterator;
    }

    Iterator& operator-=(difference_type n)
    {
        multi_dimensional_iterator_core_access::advance(_iterator,
                                                        _dimension, -n);
        return _iterator;
    }

    Iterator operator+(difference_type x) const
    {
        Iterator result(_iterator);
        return result(_dimension) += x;
    }

    Iterator operator-(difference_type x) const
    {
        Iterator result(_iterator);
        return result(_dimension) -= x;
    }
};


// Implementation of a multi dimensional iterator interface
template <class Derived,
          class Value,
          class CategoryOrTraversal,
          class Reference,
          class Difference,
          class Index>
class inca::multi_dimensional_iterator_facade
                    : public boost::iterator_facade<Derived,
                                                    Value,
                                                    CategoryOrTraversal,
                                                    Reference,
                                                    Difference> {
public:
    // Additional numeric types
    typedef Index index_type;


    // Access to an individual axis (returns an axis_iterator_facade)
    axis_iterator_facade<Derived> operator()(index_type d)
    {
        return axis_iterator_facade<Derived>(d, static_cast<Derived &>(*this));
    }
    axis_iterator_facade<Derived const> const operator()(index_type d) const
    {
        return axis_iterator_facade<Derived const>(d, static_cast<Derived const &>(*this));
    }
};


/*---------------------------------------------------------------------------*
 | axis_iterator_facade non-member operators
 *---------------------------------------------------------------------------*/
namespace inca {
    // Proxy/integer addition: n + i
    template <class Iterator>
    inline Iterator operator+(typename Iterator::difference_type n,
                              axis_iterator_facade<Iterator> const &f)
    {
        return f + n;
    }
};

#endif

