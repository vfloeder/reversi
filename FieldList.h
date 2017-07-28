//
// Copyright (c) 2017 Volker Floeder
// All rights reserved.
//
// Redistribution and use in source and binary forms are permitted
// provided that the above copyright notice and this paragraph are
// duplicated in all such forms and that any documentation,
// advertising materials, and other materials related to such
// distribution and use acknowledge that the software was developed
// by Volker Floeder. The name of Volker Floeder may not be used
// to endorse or promote products derived from this software without
// specific prior written permission.
// THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
//

#ifndef FIELDLIST_H
#define FIELDLIST_H

#include "FieldValue.h"

// =====================================================================================================================

/// @brief list of field-values (which is a list as well), used to store the currently possible moves for the game

class FieldList {
public:
    /*! @brief get index of position with the highest number of possible flips
     *
     * @return      index
     */
    int getBestPos() const
    {
        int ret { -1 };
        int v { -1 };

        for( int idx { 0 }; idx < m_Values.size(); ++idx )
        {
            const int curVal { m_Values[idx].getValue() };

            if( curVal > v ) {
                ret = idx;
                v = curVal;
            }
        }
        return ret;
    }

    /*! @brief iterator regarding list of Field-Values
     *
     * @return      iterator
     */
    std::vector<FieldValue>::const_iterator begin() const
    { return m_Values.begin(); }

    /*! @brief iterator regarding list of Field-Values
     *
     * @return      iterator
     */
    std::vector<FieldValue>::const_iterator end() const
    { return m_Values.end(); }

    /*! @brief get number of positions
     *
     * @return      number of positions
     */
    size_t size() const
    { return  m_Values.size(); }

    /*! @brief index operator
     *
     * @param i     index to access
     * @return      reference to stored value (list of positions)
     */
    FieldValue& operator[]( size_t i )
    { return m_Values[i]; }

    /*! @brief add an element to the list
     *
     * @param elem      element to add
     */
    void push_back( const FieldValue& elem )
    { m_Values.push_back(elem); }

    /*! access last element of lisst
     *
     * @return          reference to last element of list
     */
    const FieldValue& back() const
    { return m_Values.back(); }

    /*! @brief remove last element from list
     *
     */
    void pop_back()
    { m_Values.pop_back(); }
private:
    std::vector<FieldValue>     m_Values{};                     ///< the list itself
};

#endif //FIELDLIST_H
