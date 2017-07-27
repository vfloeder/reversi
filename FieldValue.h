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

#ifndef FIELDVALUE_H
#define FIELDVALUE_H

#include <vector>
#include <cstddef>

#include "Pos_Vect.h"

// =====================================================================================================================

// class to represent the "value" of a field of the game-board by means of the number of stones that
// will get flipped if a stone of a certain color is placed on that field.
//
// For easier handling, we do not only store the number of flippable stones here, but the complete list
// of positions of those stones.

class FieldValue {
public:
    explicit FieldValue( const Pos_Vect& position )
        : m_position { position }
    {}

    // get position of the field / stone
    Pos_Vect getPosition() const
    { return m_position; }

    void setPosition( const Pos_Vect& pos )
    { m_position = pos; }

    // get the number of possible flips
    int getValue() const
    { return static_cast<int>(m_Flips.size()); }

    // add the position of a possible flip
    void addPosition( const Pos_Vect& pos )
    { m_Flips.push_back(pos); }

    std::vector<Pos_Vect>::iterator begin()
    { return { m_Flips.begin() }; }

    std::vector<Pos_Vect>::iterator end()
    { return { m_Flips.end() }; }

    // remove all possible flips, setting the value to 0 (invalid move)
    void delPostions()
    { m_Flips.clear(); }

    // add another field / value to this one, effectively raising the "score"
    // - used to combine analysis of different directions
    FieldValue& operator +=( const FieldValue& v )
    {
        for( const auto &p : v.m_Flips )
        {
            addPosition(p);
        }
		return *this;
    }
private:
    Pos_Vect                m_position { 0, 0 };                    ///< position of stone / field
    std::vector<Pos_Vect>   m_Flips {};                             ///< list of stones that would be flipped
};

#endif //FIELDVALUE_H
