//
// Copyright (c) 2017 Volker Floeder
// All rights reserved.
//

#ifndef POS_VECT_H
#define POS_VECT_H

// =====================================================================================================================

/*! @brief position / vector / direction on the board
 * @details This class is used for easier analysation of possible moves since both actual position and
 * "looking" direction can be coded as a Pos_Vect, which can be easily added and substracted.
 *
 * It contains
 * - the coordinates
 *
 * It implements
 * - assignment
 * - addition
 * - substraction
 * - comparison
 * - getter
 */

class Pos_Vect
{
public:
    Pos_Vect() = default;

    ~Pos_Vect() = default;

    /*! @brief constructor
     *
     * @param x     x coordinate
     * @param y     y coordinate
     */
    Pos_Vect( const int x, const int y )
        : m_x { x }
        , m_y { y }
    {}

    /*! @brief copy constructor
     *
     */
    Pos_Vect( const Pos_Vect& ) = default;
    /*! @brief assignment operator
     *
     * @return  self-reference
     */
    Pos_Vect& operator=( const Pos_Vect& ) = default;

    /*! @brief addition
     *
     * @param toAdd     vector to add
     * @return          resulting vector
     */
    Pos_Vect operator+( const Pos_Vect& toAdd ) const
    {
        Pos_Vect ret { m_x + toAdd.m_x, m_y + toAdd.m_y };

        return ret;
    }

    /*! @brief comparison
     *
     * @param toCheck   vector to compare to
     * @return          true if not equal
     */
    bool operator!=( const Pos_Vect& toCheck ) const
    {
        return m_x != toCheck.m_x || m_y != toCheck.m_y;
    }

    /*! @brief substraction
     *
     * @param toSub     vector to substract
     * @return          resulting vector
     */
    Pos_Vect operator-( const Pos_Vect& toSub )
    {
        Pos_Vect ret { m_x - toSub.m_x, m_y - toSub.m_y };

        return ret;
    }

    /*! @brief self-addition
     *
     * @param toAdd     vector to add
     * @return          self-reference
     */
    Pos_Vect& operator+=( const Pos_Vect& toAdd )
    {
        m_x += toAdd.m_x;
        m_y += toAdd.m_y;

        return *this;
    }

    /*! @brief self-substraction
     *
     * @param toSub     vector to substract
     * @return          self-reference
     */
    Pos_Vect& operator-=( const Pos_Vect& toSub )
    {
        m_x -= toSub.m_x;
        m_y -= toSub.m_y;

        return *this;
    }

    /*! @brief get x-coordinate
     *
     * @return      x-coordinate
     */
    int getX() const
    { return m_x; }

    /*! @brief get y-coordinate
     *
     * @return      y-coordinate
     */
    int getY() const
    { return m_y; }

private:
    int m_x { 0 };                  ///< x-coordinate
    int m_y { 0 };                  ///< y-coordinate
};


#endif //POS_VECT_H
