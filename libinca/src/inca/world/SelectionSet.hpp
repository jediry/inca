/*
 * File: SelectionSet.hpp
 *
 * Author: Ryan L. Saunders
 *
 * Copyright 2004, Ryan L. Saunders. Permission is granted to use and
 *      distribute this file freely for educational purposes.
 *
 * Description:
 *      The SelectionSet class represents a selection of items.
 *      Selected-ness is a binary feature of a the items: an item
 *      is either selected or it is not. Essentially, a SelectionSet is
 *      glorified set.
 *
 *      SelectionSet provides all of the normal set operations (replace,
 *      union, intersection, difference, symmetric difference, set complement)
 *      and tests (set (in)equality, super/sub-set, proper super/sub-set).
 *      In addition, most of these operations are also accessible as
 *      operator overloads:
 *
 *          Operation       Non-mutating    Mutating
 *          =============   ============    ========
 *          Replace:        s2 = s1;
 *          Union:          s3 = s1 + s2    s2 += s1
 *          Intersection:   s3 = s1 ^ s2    s2 ^= s1
 *          Difference:     s3 = s1 - s2    s2 -= s1
 *          Symm. Diff.:    s3 = s1 % s2    s2 %= s1
 *          Complement:     s2 = !s1
 *
 *
 *          Test        Operator        Test                Operator
 *          ==========  ========        ===============     ========
 *          Equality    s1 == s2        Inequality          s1 != s2
 *          Superset    s1 >= s2        Strict superset     s1 >  s2
 *          Subset      s1 <= s2        Strict subset       s1 <  s2
 */

#ifndef INCA_WORLD_SELECTION
#define INCA_WORLD_SELECTION

// Import library configuration
#include <inca/inca-common.h>

// This is part of the Inca world object model
namespace inca {
    namespace world {
        // Forward declarations
        class SelectionSet;

        // Pointer typedefs
        typedef shared_ptr<SelectionSet>       SelectionSetPtr;
        typedef shared_ptr<SelectionSet const> SelectionSetConstPtr;
    };
};


// Import container defintions
#include <set>


class inca::world::SelectionSet {
/*---------------------------------------------------------------------------*
 | Type definitions
 *---------------------------------------------------------------------------*/
protected:
    // Container typedefs
    typedef std::set<IDType>  IDSet;

public:
    // Iterator typedefs -- passed up from the set
    typedef IDSet::iterator         iterator;
    typedef IDSet::const_iterator   const_iterator;

protected:
    IDSet items; // The set of things that are selected


/*---------------------------------------------------------------------------*
 | Iterator access
 *---------------------------------------------------------------------------*/
public:
    const_iterator begin() const { return items.begin(); }
          iterator begin()       { return items.begin(); }
    const_iterator end() const { return items.end(); }
          iterator end()       { return items.end(); }


/*---------------------------------------------------------------------------*
 | Element operations & tests
 *---------------------------------------------------------------------------*/
public:
    SizeType size() const;              // How many items in the selection?
    void select(IDType id);               // Make this item selected
    void deselect(IDType id);             // Make this item unselected
    bool isSelected(IDType id) const;     // Is this item selected?
    void setSelected(IDType id, bool s);  // Make it so


/*---------------------------------------------------------------------------*
 | Set operations & tests
 *---------------------------------------------------------------------------*/
public:
    // Mutating set operation functions
    void clear();
    void unionWith(const SelectionSet &s);
    void intersectWith(const SelectionSet &s);
    void differenceWith(const SelectionSet &s);
    void symmetricDifferenceWith(const SelectionSet &s);
    void complement();
    void selectAll();

    // Set test functions
    bool isEqualTo(const SelectionSet &s) const;
    bool isUnequalTo(const SelectionSet &s) const;
    bool isSupersetOf(const SelectionSet &s) const;
    bool isSubsetOf(const SelectionSet &s) const;
    bool isStrictSupersetOf(const SelectionSet &s) const;
    bool isStrictSubsetOf(const SelectionSet &s) const;
    bool isEmpty() const { return items.empty(); }

    // Cast-to-bool is a synonym for 'not empty?'
    operator bool() const { return ! isEmpty(); }
};


/*---------------------------------------------------------------------------*
 | Operator overloads for working with SelectionSets (they just pass thru to the
 | member functions implemented in SelectionSet). These are implemented as
 | template operators so that they can correctly call overloaded functions
 | in subclasses of SelectionSet without the need to make them virtual, and also
 | in order to generate the appropriate return type for functions that
 | return a SelectionSet. Binary operators require that the two arguments be
 | of the same type.
 *---------------------------------------------------------------------------*/

// Import template metaprogramming constructs
#include <inca/util/metaprogramming/macros.hpp>

#define SEL_1_TEMPLATE  template <class _Sel2>
#define SEL_2_TEMPLATE  template <class _Sel1, class _Sel2>
#define SEL_LHS         _Sel1
#define SEL_RHS         _Sel2
#define SEL_1_RETURN    ENABLE_IF_T( IS_A( SEL_RHS, inca::world::SelectionSet), SEL_RHS )
#define SEL_2_RETURN    ENABLE_IF_T( AND2( IS_A( SEL_LHS, inca::world::SelectionSet), IS_SAME(SEL_LHS, SEL_RHS)), SEL_RHS )
#define BOOL_RETURN     ENABLE_IF_T( AND2( IS_A( SEL_LHS, inca::world::SelectionSet), IS_SAME(SEL_LHS, SEL_RHS)), bool )


namespace inca {
    namespace world {
        /*-------------------------------------------------------------------*
         | Mutating (computed assignment) set operators
         *-------------------------------------------------------------------*/
        // Union-with operator
        SEL_2_TEMPLATE
        SEL_2_RETURN & operator+=(SEL_LHS & s1, const SEL_RHS & s2) {
            s1.unionWith(s2);
            return s1;
        }

        // Intersection-with operator
        SEL_2_TEMPLATE
        SEL_2_RETURN & operator^=(SEL_LHS & s1, const SEL_RHS & s2) {
            s1.intersectWith(s2);
            return s1;
        }

        // Difference-with operator
        SEL_2_TEMPLATE
        SEL_2_RETURN & operator-=(SEL_LHS & s1, const SEL_RHS & s2) {
            s1.differenceWith(s2);
            return s1;
        }

        // Symmetric-difference-with operator
        SEL_2_TEMPLATE
        SEL_2_RETURN & operator%=(SEL_LHS & s1, const SEL_RHS & s2) {
            s1.symmetricDifferenceWith(s2);
            return s1;
        }


        /*-------------------------------------------------------------------*
         | Non-mutating set operators
         *-------------------------------------------------------------------*/
        // Union operator
        SEL_2_TEMPLATE
        SEL_2_RETURN operator+(const SEL_LHS & s1, const SEL_RHS & s2) {
            SEL_RHS s3(s1);
            return s3 += s2;
        }

        // Intersection operator
        SEL_2_TEMPLATE
        SEL_2_RETURN operator^(const SEL_LHS & s1, const SEL_RHS & s2) {
            SEL_RHS s3(s1);
            return s3 ^= s2;
        }

        // Difference operator
        SEL_2_TEMPLATE
        SEL_2_RETURN operator-(const SEL_LHS & s1, const SEL_RHS & s2) {
            SEL_RHS s3(s1);
            return s3 -= s2;
        }

        // Symmetric-difference operator
        SEL_2_TEMPLATE
        SEL_2_RETURN operator%(const SEL_LHS & s1, const SEL_RHS & s2) {
            SEL_RHS s3(s1);
            return s3 %= s2;
        }

        // Set complement operator
        #if 0   // VS can't hack it!
        SEL_1_TEMPLATE SEL_1_RETURN operator~(const SEL_RHS &s2) {
            SEL_RHS s3(s2);
            s3.complement();
            return s3;
        }
        #endif


        /*-------------------------------------------------------------------*
         | Set test operators
         *-------------------------------------------------------------------*/
        // Set equality test
        SEL_2_TEMPLATE
        BOOL_RETURN operator==(const SEL_LHS & s1, const SEL_RHS & s2) {
            return s1.isEqualTo(s2);
        }

        // Set inequality test
        SEL_2_TEMPLATE
        BOOL_RETURN operator!=(const SEL_LHS & s1, const SEL_RHS & s2) {
            return s1.isUnequalTo(s2);
        }

        // Superset test
        SEL_2_TEMPLATE
        BOOL_RETURN operator>=(const SEL_LHS & s1, const SEL_RHS & s2) {
            return s1.isSupersetOf(s2);
        }

        // Subset test
        SEL_2_TEMPLATE
        BOOL_RETURN operator<=(const SEL_LHS & s1, const SEL_RHS & s2) {
            return s1.isSubsetOf(s2);
        }

        // Strict superset test
        SEL_2_TEMPLATE
        BOOL_RETURN operator>(const SEL_LHS & s1, const SEL_RHS & s2) {
            return s1.isStrictSupersetOf(s2);
        }

        // Strict subset test
        SEL_2_TEMPLATE
        BOOL_RETURN operator<(const SEL_LHS & s1, const SEL_RHS & s2) {
            return s1.isStrictSubsetOf(s2);
        }
    }
}


// Clean up the preprocessor namespace
#undef SEL_1_TEMPLATE
#undef SEL_2_TEMPLATE
#undef SEL_LHS
#undef SEL_RHS
#undef SEL_1_RETURN
#undef SEL_2_RETURN
#undef BOOL_RETURN

// Clean up the preprocessor's namespace
#define UNDEFINE_INCA_METAPROGRAMMING_MACROS
#include <inca/util/metaprogramming/macros.hpp>

#endif
