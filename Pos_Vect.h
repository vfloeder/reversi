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

#ifndef POS_VECT_H
#define POS_VECT_H

// =====================================================================================================================

// postion / vector / direction on the board
// used for easier analysation of possible moves since both actual position and "looking" direction
// can be coded as a Pos_Vect, which can be easily added and substracted.

class Pos_Vect
{
public:
    Pos_Vect() = default;

    ~Pos_Vect() = default;

    Pos_Vect( int x, int y )
        : m_x { x }
        , m_y { y }
    {}

    Pos_Vect( const Pos_Vect& ) = default;
    Pos_Vect& operator=( const Pos_Vect& ) = default;

    Pos_Vect operator+( const Pos_Vect& toAdd )
    {
        Pos_Vect ret { m_x + toAdd.m_x, m_y + toAdd.m_y };

        return ret;
    }

    bool operator!=( const Pos_Vect& toCheck ) const
    {
        return m_x != toCheck.m_x || m_y != toCheck.m_y;
    }

    Pos_Vect operator-( const Pos_Vect& toSub )
    {
        Pos_Vect ret { m_x - toSub.m_x, m_y - toSub.m_y };

        return ret;
    }

    Pos_Vect& operator+=( const Pos_Vect& toAdd )
    {
        m_x += toAdd.m_x;
        m_y += toAdd.m_y;

        return *this;
    }

    Pos_Vect& operator-=( const Pos_Vect& toSub )
    {
        m_x -= toSub.m_x;
        m_y -= toSub.m_y;

        return *this;
    }

    operator Pos_Vect () const
    {
        return Pos_Vect(m_x, m_y);
    }

    int getX() const
    { return m_x; }

    int getY() const
    { return m_y; }

private:
    int m_x { 0 };
    int m_y { 0 };
};


#endif //POS_VECT_H
