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

#include "Reversi.h"

// =====================================================================================================================

const Reversi::ValidDirections
    Reversi::m_Directions { Pos_Vect{ 0, 1}, Pos_Vect{ 1, 1}, Pos_Vect{ 1, 0}, Pos_Vect{ 1,-1},
                            Pos_Vect{ 0,-1}, Pos_Vect{-1,-1}, Pos_Vect{-1, 0}, Pos_Vect{-1, 1} };

// ---------------------------------------------------------------------------------------------------------------------

Reversi::Reversi( int siz )
    : m_BoardSize{siz}
    , m_Board{siz}
{
    for( int i { 0 }; i < m_BoardSize; ++i )
    {
        for( int j { 0 }; j < m_BoardSize; ++j )
        {
            m_Board.setToField({ i, j }, Stone::NoStone);
        }
    }

    // Initial board state is like:
    // ...  ... ... ... ...
    // ...  O O O O O O ...
    // ...  O O O O O O ...
    // ...  O O W B O O ...
    // ...  O O B W O O ...
    // ...  O O O O O O ...
    // ...  O O O O O O ...
    // ...  ... ... ... ...

    m_Board.setToField({ m_BoardSize / 2 - 1, m_BoardSize / 2 - 1 }, Stone::BlackStone);
    m_Board.setToField({ m_BoardSize / 2, m_BoardSize / 2 }, Stone::BlackStone);
    m_BlackStones = 2;
    m_Board.setToField({ m_BoardSize / 2 - 1, m_BoardSize / 2 }, Stone::WhiteStone);
    m_Board.setToField({ m_BoardSize / 2, m_BoardSize / 2 - 1 }, Stone::WhiteStone);
    m_WhiteStones = 2;
}

// ---------------------------------------------------------------------------------------------------------------------

Reversi::Reversi( const Reversi& other )
    : m_BoardSize { other.m_BoardSize }
    , m_Board { m_BoardSize }
{
    *this = other;
}

// ---------------------------------------------------------------------------------------------------------------------

Reversi& Reversi::operator=( const Reversi& other )
{
    if( this != &other )
    {
        if( m_BoardSize != other.m_BoardSize ) throw std::logic_error("Different board sizes");

        m_NumMoves      = other.m_NumMoves;
        m_BoardSize     = other.m_BoardSize;
        m_Board         = other.m_Board;
        m_WhiteStones   = other.m_WhiteStones;
        m_BlackStones   = other.m_BlackStones;
    }
    return *this;
}

// ---------------------------------------------------------------------------------------------------------------------

Reversi::Stone Reversi::otherColor( Reversi::Stone stone )
{
    if( stone == Stone::NoStone )
    {
        return Stone::NoStone;
    }

    return stone == Stone::BlackStone ? Stone::WhiteStone : Stone::BlackStone;
}

// ---------------------------------------------------------------------------------------------------------------------

FieldValue Reversi::checkNeighbor( Pos_Vect toCheck, Pos_Vect direction, Stone stone )
{
    Pos_Vect    neighborPos  { toCheck + direction };                       // neighbor to check
    const Stone oppositStone { otherColor(stone) };                         // look for those
    FieldValue  ret { toCheck };

    while( m_Board.isValidPostion(neighborPos))                             // is the position valid?
    {
        const Stone checkStone { m_Board.peekField(neighborPos) };          // look at that stone

        if( checkStone == oppositStone )
        {
            ret.addValuePosition(neighborPos);                                   // might get flipped
        }
        else if( checkStone == Stone::NoStone )                             // invalid position for "toCheck" for "stone"
        {                                                                   //      regarding "direction"
            ret.delPostions();
            return ret;
        }
        else
        {
            return ret;                                                     // found our own color, so OK
        }

        neighborPos += direction;                                           // go further in that direction
    }
    // boarder reached ...
    ret.delPostions();
    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------

void Reversi::setStone( const Pos_Vect& pos, Stone stone )
{
    m_Board.setToField(pos, stone);

    if( Stone::WhiteStone == stone ) ++m_WhiteStones;
    else                             ++m_BlackStones;
}

// ---------------------------------------------------------------------------------------------------------------------

void Reversi::removeStone( const Pos_Vect& pos )
{
    Stone stone = m_Board.peekField(pos);
    m_Board.setToField(pos, Stone::NoStone);

    switch( stone ) {
    case Stone::WhiteStone : --m_WhiteStones; break;
    case Stone::BlackStone : --m_BlackStones; break;
    case Stone::NoStone    : break;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void Reversi::flipStone( const Pos_Vect& pos )
{
    const Stone stone { m_Board.peekField(pos) };

    if( Stone::WhiteStone == stone )
    {
        m_Board.setToField(pos, Stone::BlackStone);
        --m_WhiteStones;
        ++m_BlackStones;
    }
    else
    {
        m_Board.setToField(pos, Stone::WhiteStone);
        --m_BlackStones;
        ++m_WhiteStones;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

FieldList Reversi::getValidMoves( Stone stone )
{
    FieldList validMoves {};

    for( int x { 0 }; x < m_BoardSize; ++x )
    {
        for( int y { 0 }; y < m_BoardSize; ++y )
        {
            Pos_Vect        curPos { x, y };
            Reversi::Stone  curStone { m_Board.peekField(curPos) };

            if( Reversi::Stone::NoStone == curStone )                       // moves are only valid for EMPTY fields
            {
                FieldValue fv{curPos};

                for( const auto& curDir : m_Directions )                    // go through all directions
                {
                    FieldValue n = checkNeighbor(curPos, curDir, stone);    // and check neighbor in that direction

                    if( n.getValue() )                                      // flips available?
                    {
                        fv += n;                                            // add them to the result
                    }
                }

                if( fv.getValue() > 0 ) {                                   // if "flippables", add position to the list
                    validMoves.push_back(fv);
                }
            }
        }
    }

    setValidMoveNum(stone, static_cast<int>(validMoves.size()));

    return validMoves;
}

// ---------------------------------------------------------------------------------------------------------------------

void Reversi::setValidMoveNum( Stone stone, int moveNum )
{
    if( Stone::WhiteStone == stone )
    {
        m_NumMoves.m_WhiteMoves = moveNum;                                  // store number of valid moves
    }
    else
    {
        m_NumMoves.m_BlackMoves = moveNum;
    }
}
