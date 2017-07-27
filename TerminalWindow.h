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

#ifndef TERMINALWINDOW_H
#define TERMINALWINDOW_H

#include <sstream>
#include <vector>

#include "curses.h"

#include "FieldValue.h"
#include "Pos_Vect.h"
#include "QuadraticBoard.h"

// =====================================================================================================================

// base class, used to enable printing of objects in the curses window
class WindowObject
{
public:
    virtual void print() = 0;
};

// =====================================================================================================================

// ncurses window to "graphically" view the game-board and information
class TerminalWindow
{
public:
    TerminalWindow( int cols, int rows );

    ~TerminalWindow();

    // set special char value to mark an "empty field"
    void setEmptyChar( int emptyField )
    { m_EmptyField = emptyField; }

    // add an object to the list of self-drawing parts
    void addObject( WindowObject* obj );

    // initialize the display
    void initWin();

    // rlease the display
    void releaseWin();

    // move cursor to a position
    void tmove( const Pos_Vect& pos )
    { wmove(m_Win, pos.getY(), pos.getX()); }

    // add (set) a character at the current cursor-position
    void taddch( const int ch )
    { waddch(m_Win, ch); }

    // add a string at the current cursor-position
    void taddstr( const char* str )
    { waddstr(m_Win, str); }

    int getEmptyChar() const
    { return m_EmptyField; }

protected:
#if ! defined(PDCURSES)
    // set the size of the terminal window
    void setSize( int cols, int rows );

    // get the size of the terminal window
    void getSize();
#endif

private:
    int                         m_EmptyField;                   // to draw if field empty
    WINDOW*                     m_Win;                          // ncurses handle
    int                         m_Cols;                         // coloumns to view
    int                         m_Rows;                         // rows to view
    int                         m_OrgCols { -1 };               // number of coloumns of "original" terminal window
    int                         m_OrgRows { -1 };               //           rows
    std::vector<WindowObject*>  m_Objects {};                   // list of objects to print

    static constexpr const int  m_KeyboadTimeoutMs { 50 };      // key-board wait timeout in milliseconds

};

#endif //TERMINALWINDOW_H
