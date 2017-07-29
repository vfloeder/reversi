//
// Copyright (c) 2017 Volker Floeder
// All rights reserved.
//

#include "CursesGrid.h"

// =====================================================================================================================

CursesGrid::CursesGrid( TerminalWindow& term, int size )
    : m_HorOffset { 3 }
    , m_VertOffset { 1 }
    , m_TermWin { term }
    , m_GridSize { size }
    , m_Grid {size}
{
    const int empty { m_TermWin.getEmptyChar() };

    for( int x { 0 }; x < m_GridSize; ++x )
    {
        for( int y { 0 }; y < m_GridSize; ++y )
        {
            m_Grid.setToField({ x, y }, empty);                              // cell initially empty
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void CursesGrid::printStatus( const std::string& status )
{
    move(0, 0);
    clrtoeol();

    move(0, 0);
    addstr(status.c_str());
}

// ---------------------------------------------------------------------------------------------------------------------

void CursesGrid::printHelp( std::vector<std::string> lines )
{
    clear();
    refresh();

    for( int i = 0; i < lines.size(); ++i )
    {
        move(i, 0);
        addstr(lines[i].c_str());
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void CursesGrid::setCursor( const Pos_Vect& pos )
{
    const int numLines { m_GridSize };
    const int x { pos.getX() };
    const int y { pos.getY() };

    const Pos_Vect termPos {m_HorOffset + ( 1 + x * 2 ), m_VertOffset + ( numLines * 2 - y * 2 - 1 )};

    m_TermWin.tmove(termPos);

}

// ---------------------------------------------------------------------------------------------------------------------

void CursesGrid::markCell( const Pos_Vect& pos, bool reverse )
{
    int newChar { m_Grid.peekField(pos) };

    if( reverse )
    {
        newChar |= COLOR_PAIR(2);
        newChar &= ~A_REVERSE;
    }
    else
    {
        newChar |= COLOR_PAIR(3);
    }

    setChar(pos, newChar);
    refresh();
}

// ---------------------------------------------------------------------------------------------------------------------

void CursesGrid::unmarkCell( const Pos_Vect& pos, bool reverse )
{
    int newChar { m_Grid.peekField(pos) };

    if( reverse )
    {
        newChar &= ~COLOR_PAIR(2);
        newChar |= A_REVERSE;
    }
    else
    {
        newChar &= ~COLOR_PAIR(3);
    }

    setChar(pos, newChar);
    refresh();
}

// ---------------------------------------------------------------------------------------------------------------------

void CursesGrid::setChar( const Pos_Vect& pos, int ch )
{
    setCursor(pos);
    m_TermWin.taddch(ch);

    m_Grid.setToField(pos, ch);

    setCursor(pos);
}

// ---------------------------------------------------------------------------------------------------------------------

void CursesGrid::print()
{
    std::stringstream sstr;
    const int         numCols  { m_GridSize };
    const int         numLines { m_GridSize };

    clear();
    // print upper grid line
    m_TermWin.tmove({m_HorOffset, m_VertOffset});
    m_TermWin.taddch(ACS_ULCORNER);

    for( int i { 0 }; i < numCols - 1; ++i )
    {
        m_TermWin.taddch(ACS_HLINE);
        m_TermWin.taddch(ACS_TTEE);
    }
    m_TermWin.taddch(ACS_HLINE);
    m_TermWin.taddch(ACS_URCORNER);

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // print individual fields for all lines and columns

    for( int i { 0 }; i < numLines; ++i )
    {
        const int boardY  { numLines - 1 - i };
        const int screenY { m_VertOffset + (2 * i + 1) };

        // print numbers of this row
        m_TermWin.tmove({1, screenY});
        m_TermWin.taddch('0' + boardY);

        // print line with cell values and horizontal delimiters
        m_TermWin.tmove({m_HorOffset, screenY});
        m_TermWin.taddch(ACS_VLINE);

        for( int col1 { 0 }; col1 < numCols; ++col1 )
        {
            const int curChar { m_Grid.peekField({col1, boardY}) };

            m_TermWin.taddch(curChar);
            m_TermWin.taddch(ACS_VLINE);
        }

        // print intermediate grid line
        m_TermWin.tmove({m_HorOffset, screenY + 1});
        m_TermWin.taddch(ACS_LTEE);

        for( int col2 { 0 }; col2 < numCols - 1; ++col2 )
        {
            m_TermWin.taddch(ACS_HLINE);
            m_TermWin.taddch(ACS_PLUS);
        }
        m_TermWin.taddch(ACS_HLINE);
        m_TermWin.taddch(ACS_RTEE);
    }

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // print lower grid line
    m_TermWin.tmove({0, m_VertOffset + (2 * numLines)});
    m_TermWin.taddstr("   ");
    m_TermWin.taddch(ACS_LLCORNER);

    for( int i { 0 }; i < numCols - 1; ++i )
    {
        m_TermWin.taddch(ACS_HLINE);
        m_TermWin.taddch(ACS_BTEE);
    }
    m_TermWin.taddch(ACS_HLINE);
    m_TermWin.taddch(ACS_LRCORNER);

    // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    // print number of columns
    m_TermWin.tmove({m_HorOffset, m_VertOffset + ( 2 * numLines + 1 )});

    for( int i { 0 }; i < numCols; ++i )
    {
        m_TermWin.taddch(' ');
        m_TermWin.taddch('0' + i);
    }

}

// ---------------------------------------------------------------------------------------------------------------------

// we can only print values 1 .. 9, for larger values we print a '+' instead
void CursesGrid::markCells( const FieldList& fieldList, bool reverse )
{
    for( const auto& idx : fieldList )
    {
        const Pos_Vect pos  { idx.getFieldPosition() };
        const int      val  { static_cast<int>(idx.getValue() > 9 ?  '+' : ('0' + idx.getValue())) };
        const int      show { reverse ? static_cast<int>( val | A_REVERSE ) : static_cast<int>( val ) };

        setChar(pos, show);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void CursesGrid::unmarkCells( const FieldList& fieldList )
{
    const int empty { m_TermWin.getEmptyChar() };

    for( const auto& idx : fieldList )
    {
        const Pos_Vect pos { idx.getFieldPosition() };

        setChar(pos, empty);
    }
}
