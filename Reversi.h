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

class Reversi
{
public:

    // type of stones used to play, for easier handling "no stone" is also used here
    // so that we assume that each cell/field of the board initially has a "no stone"
    // type which can be exchanged by another type of stone.

    enum class Stone
    {
        BlackStone,
        NoStone,
        WhiteStone
    };

    // =================================================================================================================

    // construct game with a certain board-size
    explicit Reversi( int siz );

    // check if game is over (no further move possible)
    bool gameOver() const
    { return !m_NumMoves.m_BlackMoves && !m_NumMoves.m_WhiteMoves; }

    // get stone of opposite color
    static Stone otherColor( Stone stone );

    QuadraticBoard<Stone>::Iterator begin()
    { return m_Board.begin(); }

    QuadraticBoard<Stone>::Iterator end()
    { return m_Board.end(); }

    // check field regarding placed stone
    Stone peekField( const Pos_Vect& pos ) const
    { return m_Board.peekField(pos); }

    // put a stone on a board-field
    void setStone( const Pos_Vect& pos, Stone stone );

    // remove a stone from a field - needed to undo moves
    void removeStone( const Pos_Vect& pos );

    // flip a stone on a field, so that it gets the "other" color
    void flipStone( const Pos_Vect& pos );

    // get a complete list of all valid moves for a color at a certain state of the game
    // returns a list of "FieldValues", containing the position of the move and the list
    // of positions of stones that will be flipped if the move is choosen.
    FieldList getValidMoves( Stone stone );

    // copy constructor
    Reversi( const Reversi& other );

    // assignment operator
    Reversi& operator=( const Reversi& other );

    int getWhiteNum() const
    { return m_WhiteStones; }

    int getBlackNum() const
    { return m_BlackStones; }

protected:

    // check neigbours of a stone regarding a certain direction, returning a list of positions of
    // "captured" stones of the opposite color.
    FieldValue checkNeighbor( Pos_Vect toCheck, Pos_Vect direction, Stone stone );

    // store number of possible moves for a particular player (stone) - needed to check if game is over
    void setValidMoveNum( Stone stone, int moveNum );

private:
    // game is over if neither WHITE nor BLACK can place a stone
    struct NumMoves {
        int     m_WhiteMoves { -1 };
        int     m_BlackMoves { -1 };
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

    using ValidDirections = std::vector<Pos_Vect>;
    static const ValidDirections    m_Directions;                           ///< allowed directions

};

#endif //REVERSI_H
