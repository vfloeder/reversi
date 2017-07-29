//
// Copyright (c) 2017 Volker Floeder
// All rights reserved.
//

#ifndef QUADRATICBOARD_H
#define QUADRATICBOARD_H

#include <vector>
#include <stdexcept>
#include <sstream>

#include "Pos_Vect.h"

// =====================================================================================================================

/*! @brief quadratic board
 *
 * @tparam FieldType    type of each field / cell
 */
template<typename FieldType>
class QuadraticBoard
{
    static constexpr const int m_MinBoardSize { 4 };                        ///< minimum allowed size
    static constexpr const int m_MaxBoardSize { 10 };                       ///< maximum allowed size

public:

    using BoardOfFields = std::vector<std::vector<FieldType>>;              ///< the board type

    /*! @brief special iterator to iterate over the complete board
     *
     */
    class Iterator
    {
    public:
        /*! @brief constructor
         *
         * @param board     board to iterate
         * @param position  current position
         * @param size      size - number of cells in row and coloumn
         */
        Iterator( BoardOfFields& board, const Pos_Vect& position, int size );

        /*! @brief de-refencing
         *
         * @return      referenced field / cell
         */
        FieldType& operator*()
        { return m_Board[m_CurPos.getX()][m_CurPos.getY()]; }

        /*! @brief pre-increment
         *
         * @return      self-reference
         */
        Iterator& operator++();

        /*! @brief comparison
         *
         * @param it    iterator
         * @return      true if not equal
         */
        bool operator!=( const Iterator& it ) const
        { return m_CurPos != it.m_CurPos; }

        /*! @brief get current board-position
         *
         * @return      position
         */
        Pos_Vect getPosition() const
        { return m_CurPos; }

    private:
        BoardOfFields&  m_Board;                            ///< reference to board
        Pos_Vect        m_CurPos;                           ///< current position
        const int       m_Coloumns;                         ///< coloumn-size
        const int       m_Rows;                             ///< row-size
    };

    /*! @brief constructor
     *
     * @param size      size of board
     */
    explicit QuadraticBoard( int size );

    /*! @brief copy constructor
     *
     * @param b     board to copy
     */
    QuadraticBoard( const QuadraticBoard& b )
    : m_BoardSize { 0 }
    { *this = b; }

    /*! @brief assignment operator
     *
     * @param b     board to assign
     * @return      self-reference
     */
    QuadraticBoard& operator=( const QuadraticBoard& b );

    /*! @brief check if a position is valid, by means of within range of x-coord and y-coord
     *
     * @param pos       position to check
     * @return          true if "on the board"
     */
    bool isValidPostion( const Pos_Vect& pos ) const;

    /*!  @brief get a stone (clone) from a certain position for further analysation
     *
     * @param pos       position / cell to check
     * @return          stone
     */
    FieldType peekField( const Pos_Vect& pos ) const
    { return m_Board[pos.getX()][pos.getY()]; }

    /*! @brief begin-operator
     *
     * @return          iterator describing the first valid position on the board
     */
    Iterator begin()
    { return { m_Board, { 0, 0 }, m_BoardSize }; }

    /*! @brief end-operator describing the first invalid position regarding the board (off-board)
     * y moves faster, so x out-of-range marks the end
     * @return          iterator "off-board"
     */
    Iterator end()
    { return { m_Board, { m_BoardSize, 0 }, m_BoardSize }; }

    /*! @brief put a "stone" on a field at position ...
     *
     * @param pos       position of field
     * @param stone     value / stone to set
     */
    void setToField( const Pos_Vect& pos, FieldType stone )
    { m_Board[pos.getX()][pos.getY()] = stone; }

private:
    int           m_BoardSize;            ///< size / ranges
    BoardOfFields m_Board;                ///< board of ...
};

// ---------------------------------------------------------------------------------------------------------------------

template< typename FieldType>
QuadraticBoard<FieldType>::Iterator::Iterator( BoardOfFields& board, const Pos_Vect& position, const int size )
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

// =====================================================================================================================

template< typename FieldType>
QuadraticBoard<FieldType>::QuadraticBoard( const int size )
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

template< typename FieldType>
QuadraticBoard<FieldType>&  QuadraticBoard<FieldType>::operator=( const QuadraticBoard& b )
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
#endif //QUADRATICBOARD_H
