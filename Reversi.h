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

#ifndef REVERSI_H
#define REVERSI_H

/*! @mainpage Reversi with <b><em>pdcurses</em></b> gui
 * A typical screen looks like this:
 * @image html PDCurses_005.png
 * and here is a diagram of how the game is implemented:
 * @image html class-diagram.png
 *
 */

#include <array>
#include <vector>
#include <cstddef>
#include <stdexcept>
#include <iostream>
#include <iomanip>

#include "Pos_Vect.h"
#include "FieldValue.h"
#include "FieldList.h"
#include "QuadraticBoard.h"

// =====================================================================================================================

/*! @brief basic game implementation
 *
 */
class Reversi
{
public:

    /*! type of stones used to play, for easier handling "no stone" is also used here
     * so that we assume that each cell/field of the board initially has a "no stone"
     * type which can be exchanged by another type of stone.
     */
    enum class Stone
    {
        BlackStone,
        NoStone,
        WhiteStone
    };

    // =================================================================================================================

    /*! @brief construct game with a certain board-size
     *
     *  @param siz      size of the board
     */
    explicit Reversi( int siz );

    /*! @brief check if game is over (no further move possible)
     *
     * @return          true if game is over
     */
    bool gameOver() const
    { return !m_NumMoves.m_BlackMoves && !m_NumMoves.m_WhiteMoves; }

    /*! @brief get stone of opposite color
     *
     * @param stone     stone to get opposite of
     * @return          stone of opposite color
     */
    static Stone otherColor( Stone stone );

    /*! @brief iterator
     *
     * @return          iterator regaring fields / cells
     */
    QuadraticBoard<Stone>::Iterator begin()
    { return m_Board.begin(); }

    /*! @brief iterator
     *
     * @return          iterator regaring fields / cells
     */
    QuadraticBoard<Stone>::Iterator end()
    { return m_Board.end(); }

    /*! @brief check field regarding placed stone
     *
     * @param pos       position to check
     * @return          stone at that position
     */
    Stone peekField( const Pos_Vect& pos ) const
    { return m_Board.peekField(pos); }

    /*! @brief put a stone on a board-field
     *
     * @param pos       position where to place the stone
     * @param stone     stone to use
     */
    void setStone( const Pos_Vect& pos, Stone stone );

    /*! @brief remove a stone from a field - needed to undo moves
     *
     * @param pos       position
     */
    void removeStone( const Pos_Vect& pos );

    /*! @brief flip a stone on a field, so that it gets the "other" color
     *
     * @param pos       position
     */
    void flipStone( const Pos_Vect& pos );

    /*! @brief get a complete list of all valid moves for a color at a certain state of the game
     * @details returns a list of "FieldValues", containing the position of the move and the list
     * of positions of stones that will be flipped if the move is choosen.
     * @param stone     stone color to check
     * @return          list of valid moves (positions to place that stone)
     */
    FieldList getValidMoves( Stone stone );

    /*! @brief copy constructor
     *
     * @param other     game to copy
     */
    Reversi( const Reversi& other );

    /*! @brief assignment operator
     *
     * @param other     game to assign
     * @return          self-reference
     */
    Reversi& operator=( const Reversi& other );

    /*! @brief get number of white stones on the board
     *
     * @return      number
     */
    int getWhiteNum() const
    { return m_WhiteStones; }

    /*! @brief get number of black stones on the board
     *
     * @return      number
     */
    int getBlackNum() const
    { return m_BlackStones; }

    /*! @brief get the number of fields
     *
     * @return      number of cells
     */
    int getBoardSize() const
    { return m_BoardSize * m_BoardSize; }
protected:

    /*! @brief check neigbours of a stone regarding a certain direction, returning a list of positions
     * of "captured" stones of the opposite color.
     *
     * @param toCheck       position
     * @param direction     direction to go
     * @param stone         stone / color to check
     * @return              list of "captured" stones if move was made
     */
    FieldValue checkNeighbor( Pos_Vect toCheck, Pos_Vect direction, Stone stone );

    /*! @brief store number of possible moves for a particular player (stone) - needed to check if game is over
     *
     * @param stone         stone / color
     * @param moveNum       currently possible moves
     */
    void setValidMoveNum( Stone stone, int moveNum );

private:
    /// game is over if neither WHITE nor BLACK can place a stone
    struct NumMoves {
        int     m_WhiteMoves { -1 };                                        ///< possible moves for white
        int     m_BlackMoves { -1 };                                        ///< possible moves for black
    };

    NumMoves                        m_NumMoves {};                          ///< last number of possible moves per color

    int                             m_BoardSize;                            ///< actual size of the board (4...10)
    QuadraticBoard<Stone>           m_Board;                                ///< the logical board

    int                             m_WhiteStones { 0 };                    ///< total number of white stones on the board
    int                             m_BlackStones { 0 };                    ///<                 black

    // Allowed directions for "capturing" stones, if you simply remove the "intermediate" directions like north-east ...
    // you will get a simpler version of the game.
    //
    //                          ( 0, 1)  north
    //                      ^      |
    // north-west (-1, 1) - |  NW  N   NE    - ( 1, 1)  north-east
    //                      |    \   /
    //                      |     \ /
    //       west (-1, 0) - |  W   x   E     - ( 1, 0)  east
    //                      |     / \
    //                      |    /   \
    // south-west (-1,-1) - y  SW  S  SE     - ( 1,-1)  south-east
    //                      .x --------->
    //                            |
    //                         ( 0,-1)  south

    using ValidDirections = std::vector<Pos_Vect>;                          ///< type of "valid directions"
    static const ValidDirections    m_Directions;                           ///< allowed directions

};

#endif //REVERSI_H
