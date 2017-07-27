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

// list of field-values
// used to store the currently possible moves for the game

class FieldList {
public:
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

    std::vector<FieldValue>::const_iterator begin() const
    { return m_Values.begin(); }

    std::vector<FieldValue>::const_iterator end() const
    { return m_Values.end(); }

    size_t size() const
    { return  m_Values.size(); }

    FieldValue& operator[]( size_t i )
    { return m_Values[i]; }

    void push_back( const FieldValue& elem )
    { m_Values.push_back(elem); }

    const FieldValue& back() const
    { return m_Values.back(); }

    void pop_back()
    { m_Values.pop_back(); }
private:
    std::vector<FieldValue>     m_Values{};
};

#endif //FIELDLIST_H
