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

/*! @brief class to represent the "value" of a field of the game-board by means of the number of stones that
 * will get flipped if a stone of a certain color is placed on that field.
 *
 * @details For easier handling, we do not only store the number of flippable stones here, but the complete list
 * of positions of those stones.
 */

class FieldValue {
public:
    /*! @brief constructor
     *
     * @param position          position of the field on the board
     */
    explicit FieldValue( const Pos_Vect& position )
        : m_position { position }
    {}

    /*! @brief get position of the field / stone
     *
     * @return      position
     */
    Pos_Vect getFieldPosition() const
    { return m_position; }

    /*! @brief set position
     *
     * @param pos       position to set
     */
    void setFieldPosition( const Pos_Vect& pos )
    { m_position = pos; }

    /*! @brief get the "value" by menas of the number of possible flips
     *
     * @return          value
     */
    int getValue() const
    { return static_cast<int>(m_Flips.size()); }

    /*! @brief add the position of a possible flip
     *
     * @param pos       position of a "flipable" stone
     */
    void addValuePosition( const Pos_Vect& pos )
    { m_Flips.push_back(pos); }

    /*! @brief iterator to support range-based access
     *
     * @return          iterator regarding the list of values
     */
    std::vector<Pos_Vect>::iterator begin()
    { return { m_Flips.begin() }; }

    /*! @brief iterator to support range-based access
     *
     * @return          iterator regarding the list of values
     */
    std::vector<Pos_Vect>::iterator end()
    { return { m_Flips.end() }; }

    /*! @brief remove all possible flips, setting the value to 0 (invalid move)
     *
     */
    void delPostions()
    { m_Flips.clear(); }

    /*! @brief add another field / value to this one, effectively raising the "score" - used to combine analysis of different directions
     *
     * @param v     value to add
     * @return      self-reference
     */
    FieldValue& operator +=( const FieldValue& v )
    {
        for( const auto &p : v.m_Flips )
        {
            addValuePosition(p);
        }
		return *this;
    }
private:
    Pos_Vect                m_position { 0, 0 };                    ///< position of stone / field
    std::vector<Pos_Vect>   m_Flips {};                             ///< list of stones that would be flipped
};

#endif //FIELDVALUE_H
