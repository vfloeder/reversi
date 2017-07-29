//
// Copyright (c) 2017 Volker Floeder
// All rights reserved.
//

#include <iostream>
#include <term.h>

#include "TerminalWindow.h"

// =====================================================================================================================

TerminalWindow::TerminalWindow( const int cols, const int rows )
    : m_EmptyField{ ' ' }
    , m_Cols { cols }
    , m_Rows { rows }
{
    initWin();
    noecho();                                                           // do not echo input
    start_color();

    init_pair(1, COLOR_BLACK, COLOR_GREEN);

    init_pair(2, COLOR_WHITE, COLOR_RED);
    init_pair(3, COLOR_BLACK, COLOR_RED);

    keypad(m_Win, TRUE);                                                // allow keyboard input

    timeout(m_KeyboadTimeoutMs);                                        // set timeout to wait for a keystroke
}

// ---------------------------------------------------------------------------------------------------------------------

TerminalWindow::~TerminalWindow()
{
    delwin(m_Win);
    releaseWin();
}

// ---------------------------------------------------------------------------------------------------------------------

void TerminalWindow::addObject( const WindowObject* obj )
{
    m_Objects.push_back(obj);
    refresh();
}

// ---------------------------------------------------------------------------------------------------------------------

void TerminalWindow::initWin()
{
    m_Win = initscr();

    if(( m_Win ) == nullptr )
    {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    curs_set(0);                                                        // no cursor

    for( auto obj : m_Objects )
    {
        obj->print();
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void TerminalWindow::releaseWin()
{
    endwin();
}
