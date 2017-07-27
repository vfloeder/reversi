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

#ifndef QUADRATICBOARD_H
#define QUADRATICBOARD_H

#include <vector>
#include <stdexcept>
#include <sstream>

#include "Pos_Vect.h"

// =====================================================================================================================

template<typename FieldType>
class QuadraticBoard
{
    static constexpr const int m_MinBoardSize { 4 };
    static constexpr const int m_MaxBoardSize { 10 };

public:

    using BoardOfFields = std::vector<std::vector<FieldType>>;

    class Iterator
    {
    public:
        Iterator( BoardOfFields& board, const Pos_Vect& position, int size );

        FieldType& operator*()
        { return m_Board[m_CurPos.getX()][m_CurPos.getY()]; }

        // pre-increment
        Iterator& operator++();

        bool operator!=( const Iterator& it ) const
        { return m_CurPos != it.m_CurPos; }

        Pos_Vect getPosition() const
        { return m_CurPos; }

    private:
        BoardOfFields&  m_Board;
        Pos_Vect        m_CurPos;
        const int       m_Coloumns;
        const int       m_Rows;
    };

    // constructor
    explicit QuadraticBoard( int size );

    // copy constructor
    QuadraticBoard( const QuadraticBoard& b )
    {
        *this = b;
    }

    // assignment operator
    QuadraticBoard& operator=( const QuadraticBoard& b )
    {
        if( this != &b )
        {
            m_BoardSize = b.m_BoardSize;

            for( size_t x { 0 }; x < m_BoardSize; ++x )
            {
                for( size_t y { 0 }; y < m_BoardSize; ++y )
                {
                    m_Board[x][y] = b.m_Board[x][y];
                }
            }
        }
        return *this;
    }

    // check if a position is valid, by means of within range of x-coord and y-coord
    bool isValidPostion( const Pos_Vect& pos ) const;

    // get a stone (clone) from a certain position for further analysation
    FieldType peekField( const Pos_Vect& pos ) const
    { return m_Board[pos.getX()][pos.getY()]; }

    Iterator begin()
    { return { m_Board, { 0, 0 }, m_BoardSize }; }

    // y moves faster, so x out-of-range marks the end
    Iterator end()
    { return { m_Board, { m_BoardSize, 0 }, m_BoardSize }; }

    // put a "stone" on a field at position ...
    void setField( const Pos_Vect& pos, FieldType stone )
    { m_Board[pos.getX()][pos.getY()] = stone; }

private:
    int           m_BoardSize;            ///< size / ranges
    BoardOfFields m_Board;                ///< board of ...
};

// ---------------------------------------------------------------------------------------------------------------------

template< typename FieldType>
QuadraticBoard<FieldType>::Iterator::Iterator( BoardOfFields& board, const Pos_Vect& position, int size )
: m_Board { board }
, m_CurPos { position }
, m_Coloumns { size }
, m_Rows { size }
{
}

// ---------------------------------------------------------------------------------------------------------------------

template< typename FieldType >
typename QuadraticBoard<FieldType>::Iterator&
QuadraticBoard<FieldType>::Iterator::operator++()
{
    if( m_CurPos.getY() < m_Rows - 1 )
    {
        m_CurPos = { m_CurPos.getX(), m_CurPos.getY() + 1 };
    }
    else if( m_CurPos.getX() < m_Coloumns )
    {
        m_CurPos = { m_CurPos.getX() + 1, 0 };
    }
    return *this;
}

// ---------------------------------------------------------------------------------------------------------------------

template< typename FieldType>
QuadraticBoard<FieldType>::QuadraticBoard( int size )
    : m_BoardSize { size }
    , m_Board { static_cast<size_t>(m_BoardSize), std::vector<FieldType>(m_BoardSize) }
{
    if( m_BoardSize % 2 )
    {
        throw std::logic_error("Board size must be even");
    }
    if( m_BoardSize < m_MinBoardSize )
    {
        std::stringstream sstr;

        sstr << "Board size must be at least " << m_MinBoardSize;
        throw std::logic_error(sstr.str());
    }
    if( m_BoardSize > m_MaxBoardSize )
    {
        std::stringstream sstr;

        sstr << "Board size must be at most " << m_MaxBoardSize;
        throw std::logic_error(sstr.str());
    }
}

// ---------------------------------------------------------------------------------------------------------------------

template< typename FieldType>
bool QuadraticBoard<FieldType>::isValidPostion( const Pos_Vect& pos ) const
{
    return ( pos.getX() >= 0 && pos.getX() < m_BoardSize && pos.getY() >= 0 && pos.getY() < m_BoardSize );
}

#endif //QUADRATICBOARD_H
