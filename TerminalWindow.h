//
// Copyright (c) 2017 Volker Floeder
// All rights reserved.
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

/*! @brief base class, used to enable printing of objects in the curses window
 *
 */
class WindowObject
{
public:
    virtual void print() const = 0;                   ///< child needs to implement the printing method
};

// =====================================================================================================================

/*! @brief  curses window to "graphically" view the game-board and information
 * @details This is the "outer part" of the graphic abstraction, it handles the basic curses functionality and maintains
 * a list of printable objects and the special character-code that will be displayed for empty fields of the board.
 *
 * Here you can
 * - set and get the "empty char"
 * - add a display object
 * - initialize and release the display
 * - set the output position, where the next char is shown
 * - output a single character
 * - output a string
 *
 * Additionally, the keyboard timeout is set.
 */
class TerminalWindow
{
public:
    /*! @brief constructor
     *
     * @param cols      coloumns (x)
     * @param rows      rows (y)
     */
    TerminalWindow( const int cols, const int rows );

    ~TerminalWindow();

    /*! @brief set special char value to mark an "empty field"
     *
     * @param emptyField    char to use
     */
    void setEmptyChar( const int emptyField )
    { m_EmptyField = emptyField; }

    /*! @brief add an object to the list of self-drawing parts
     *
     * @param obj       object to print
     */
    void addObject( const WindowObject* obj );

    /*! @brief initialize the display
     *
     */
    void initWin();

    /*! @brief release the display
     *
     */
    void releaseWin();

    /*! @brief move cursor to a position
     *
     * @param pos   position
     */
    void tmove( const Pos_Vect& pos )
    { wmove(m_Win, pos.getY(), pos.getX()); }

    /*! @brief add (set) a character at the current cursor-position
     *
     * @param ch    char to set / display for that position
     */
    void taddch( const int ch )
    { waddch(m_Win, ch); }

    /*! @brief add a string at the current cursor-position
     *
     * @param str   string to display
     */
    void taddstr( const char* str )
    { waddstr(m_Win, str); }

    /*! @brief get the char that is used mark an empty field
     *
     * @return      char used to mark an empty field
     */
    int getEmptyChar() const
    { return m_EmptyField; }

private:
    int                         m_EmptyField;                   ///< to draw if field empty
    WINDOW*                     m_Win;                          ///< screen handle
    int                         m_Cols;                         ///< coloumns to view
    int                         m_Rows;                         ///< rows to view

    std::vector<const WindowObject*>  m_Objects {};             ///< list of objects to print

    static constexpr const int  m_KeyboadTimeoutMs { 50 };      ///< key-board wait timeout in milliseconds

};

#endif //TERMINALWINDOW_H
