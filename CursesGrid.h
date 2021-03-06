//
// Copyright (c) 2017 Volker Floeder
// All rights reserved.
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

/*! @brief "grid" view object to be shown via curses
 * @details The class implements a grid display via curses, such that a status-line, the numbering of columns and rows and
 * some cell-separators are shown.
 *
 * It contains:
 * - the grid of characters to display
 * - the size of the grid
 * - a reference to the terminal window used for display
 * - horizontal and vertical offsets for the display - to make room for additional information to be displayed
 *
 * It implements:
 * - printing a status line
 * - printing a help text
 * - marking a cell
 * - unmarking a cell
 * - setting the display character for a sell - the cell-value
 * - printing itself via the terminal
 * - marking a list of cells
 * - unmarking a list of cells
 *
 */
class CursesGrid : public WindowObject
{
public:
    /*! @brief constructor
     *
     * @param term      terminal window (curses) used for display
     * @param size      size of the grid - N x N coloumns and rows
     */
    CursesGrid( TerminalWindow& term, const int size );

    virtual ~CursesGrid() = default;

    /*! @brief print a line containing varius information
     *
     * @param status    line to display
     */
    void printStatus( const std::string& status );

    /*!  @brief Display some help
     *
     * @param lines     lines to display
     */
    void printHelp( std::vector<std::string> lines );

    /*! @brief set the cursor to a cell position
     *
     * @param pos       position (x, y) to set the cursor to
     */
    void setCursor( const Pos_Vect& pos );

    /*! @brief mark a specific cell
     *
     * @param pos       position of cell
     * @param reverse   revert fore- and back-ground color
     */
    void markCell( const Pos_Vect& pos, const bool reverse );

    /*! @brief unmark a specific cell
     *
     * @param pos       position of cell
     * @param reverse   revert fore- and back-ground color
     */
    void unmarkCell( const Pos_Vect& pos, const bool reverse );

    /*! @brief set the character of a cell at a position
     *
     * @param pos       position
     * @param ch        character to display
     */
    void setChar( const Pos_Vect& pos, const int ch );

    /*! @brief print the grid using the terminal supplied via the constructor
     *
     */
    void print() const;

    /*! @brief mark one or more grid-cells, by displaying a special character
     *
     * @param fieldList     list of positions
     * @param reverse       revert fore- and back-ground color
     */
    void markCells( const FieldList& fieldList, const bool reverse );

    /*! @brief remove all marks from the currently marked cells
     *
     * @param fieldList     list of positions
     */
    void unmarkCells( const FieldList& fieldList );

private:
    const int                       m_HorOffset;                        ///< horizontal offset of grid-cells
    const int                       m_VertOffset;                       ///< vertical offset of cells

    TerminalWindow&                 m_TermWin;                          ///< terminal to use for display

    int                             m_GridSize;                         ///< size of the grid (it's quadratic)
    QuadraticBoard<int>             m_Grid;                             ///< the board
};

#endif //CURSESGRID_H
