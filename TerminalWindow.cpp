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

#include <iostream>
#include <term.h>

#include "TerminalWindow.h"

// =====================================================================================================================

TerminalWindow::TerminalWindow( int cols, int rows )
    : m_EmptyField{ ' ' }
    , m_Cols { cols }
    , m_Rows { rows }
{
#if ! defined(PDCURSES)
    getSize();
#endif
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
#if ! defined(PDCURSES)
    setSize(m_OrgCols, m_OrgRows);
#endif
}

// ---------------------------------------------------------------------------------------------------------------------

void TerminalWindow::addObject( WindowObject* obj )
{
    m_Objects.push_back(obj);
    refresh();
}

// ---------------------------------------------------------------------------------------------------------------------

void TerminalWindow::initWin()
{
#if ! defined(PDCURSES)
    setSize(m_Cols, m_Rows);
#endif
    m_Win = initscr();

    if(( m_Win ) == nullptr )
    {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }

    // 0, 1, 2
    curs_set(0);                                                        // set "massive" cursor

    for( auto obj : m_Objects )
    {
        obj->print();
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void TerminalWindow::releaseWin()
{
    endwin();
#if ! defined(PDCURSES)
    refresh();
    clear();
#endif
}

// ---------------------------------------------------------------------------------------------------------------------

#if ! defined(PDCURSES)
void TerminalWindow::getSize()
{
    char        buffer[2048];
    const char* termVar { getenv("TERM") };

    if( nullptr == termVar )
    {
        std::cerr << "Can't find \'TERM\' in environment" << std::endl;
        return;
    }

    int ret { tgetent(buffer, termVar) };

    if( 0 > ret )
    {
        std::cerr << "Can't access termcap data" << std::endl;
        return;
    }
    else if( 0 == ret )
    {
        std::cerr << "Terminal of type " << termVar << " isn't defined" << std::endl;
        return;
    }

    m_OrgCols = tgetnum("co");
    m_OrgRows = tgetnum("li");
}
#endif

// ---------------------------------------------------------------------------------------------------------------------

#if ! defined(PDCURSES)
void TerminalWindow::setSize( int cols, int rows )
{
    // set terminals-size
    std::stringstream sstr1;
#if( _PROJ_WINDOWS )

    sstr1 << "mode con: cols=" << t_cols << " lines=" << t_lines;
        std::string command{sstr1.str()};// = "mode con: cols=168 lines=40";
#else

    sstr1 << "resize -s " << rows << " " << cols;

    std::string command { sstr1.str() };  //{"resize -s 40 168"};
#endif

    if( 0 != system(command.c_str()))
    {
        std::cerr << "Couldn't change terminal size..." << std::endl << std::endl;
    }
}
#endif
