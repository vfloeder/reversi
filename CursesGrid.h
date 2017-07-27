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

#ifndef CURSESGRID_H
#define CURSESGRID_H

#include <string>
#include <vector>

#include "Pos_Vect.h"
#include "QuadraticBoard.h"
#include "FieldValue.h"
#include "FieldList.h"
#include "TerminalWindow.h"

// =====================================================================================================================

// "grid" view object to be shown via ncurses
class CursesGrid : public WindowObject
{
public:
    CursesGrid( TerminalWindow& term, int size );

    // print a line containing varius information
    void printStatus( const std::string& status );

    void printHelp( std::vector<std::string> lines );

    // set the cursor to a cell position
    void setCursor( const Pos_Vect& pos );

    void markField( const Pos_Vect& pos, bool reverse );

    void unmarkField( const Pos_Vect& pos, bool reverse );

    // set the character of a cell at a position
    void setChar( const Pos_Vect& pos, int ch );

    // print the grid using the terminal supplied via the constructor
    void print();

    // mark one or more grid-cells, by displaying a special character
    void markCells( const FieldList& fieldList, bool reverse );

    // remove all marks from the currently marked cells
    void unmarkCells( const FieldList& fieldList );

private:
    const int                       m_HorOffset;                        ///< horizontal offset of grid-cells
    const int                       m_VertOffset;                       ///< vertical offset of cells

    TerminalWindow&                 m_TermWin;                          ///< terminal to use for display

    int                             m_GridSize;                         ///< size of the grid (it's quadratic)
    QuadraticBoard<int>             m_Grid;
};


#endif //CURSESGRID_H
