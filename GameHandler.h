//
// Copyright (c) 2017 Volker Floeder
// All rights reserved.
//

#ifndef GAMEHANDLER_H
#define GAMEHANDLER_H

#include <atomic>
#include <mutex>
#include <condition_variable>
#include "Pos_Vect.h"
#include "FieldValue.h"
#include "CursesGrid.h"

#include "Reversi.h"

// =====================================================================================================================

/*! @brief game play implementation, all the logic is here
 * @details  The GameHandler class act on the Reversi and the GridView class. It implements the game logic and it make sure that
 * the current state of the game is displayed on the screen. However, it also contains logic to <b>not</b> display
 * intermediate calculation steps that might be done on the Reversi class.
 *
 * it contains:
 * - a refence to the Reversi class
 * - a reference to the Grid view class
 * - the current position regarding a move-selection
 * - a lsit of valid moves for a time
 * - an undo list, to undo all done moves
 *
 * it implements:
 * - a check if the game has ended
 * - get the number of white/black stones currently on the board
 * - preparing the next move (get the list of legal moves)
 * - select one from the list of possible next moves
 * - making a move
 * - undo a move
 * - get the possible flips for a selected move
 * - compute the best next move
 * The computation of the best next move is done via a min-max algorithm that computes all moves down to a
 * certain depth. This is done via an async thread that may be forced to stop by a user input.
 */
class GameHandler
{
public:
    /// @brief info to be returned by the next computed move
    struct MoveInfo {
        Pos_Vect pos;                                                               ///< position for stone
        int      idx;                                                               ///< index of move in the list of
                                                                                    ///      valid moves
    };

    /*! @brief constructor
     *
     * @param gridView      display
     * @param reversi       game
     */
    GameHandler( CursesGrid&  gridView,  Reversi& reversi );

    /*! @brief game ended?
     *
     * @return      true if game over
     */
    bool ended() const;

    /*! @brief get current number of white stones
     *
     * @return      number of white stones on the board
     */
    int getWhiteStones() const;

    /*! @brief get number of black stones
     *
     * @return      number of black stones on the board
     */
    int getBlackStones() const;

    /*! @brief prepare for the next move (calculate possibilities...)
     *
     * @param stone     stone to place
     * @param view      flag, if true the result shall be shown on the display
     * @return          true if any move is possible
     */
    bool prepareNextMove( const Reversi::Stone stone, const bool view = true );

    /*! @brief set selection of move to the next possibility, iterating of the list of possible moves
     *
     * @param stone     stone to place
     * @param view      flag, if true the result shall be shown on the display
     */
    void selectNextValidMove( const Reversi::Stone stone, const bool view = true );

    /*! @brief set to dedicated position - used for computed moves
     *
     * @param stone     stone to place
     * @param idx       index into list of possible moves
     */
    void selectValidMove( const Reversi::Stone stone, const int idx );

    /*! @brief select a stone position, making the move
     *
     * @param stone     stone to place
     * @param view      flag, if true the result shall be shown on the display
     */
    void makeMove( const Reversi::Stone  stone, const bool view = true );

    /*! @brief undo the last move
     *
     * @param view      flag, if true the result shall be shown on the display
     * @return
     */
    bool undoMove( const bool view = true );

    /*! @brief get possible flips for the position of a move
     *
     * @return          number of possible flips (catches)
     */
    int getPossibleFlips();

    /*! @brief compute a "good" next move by analysing all possibilities down to a certain depth, does an alpha-beta search
     *
     * @param stone     stone to place
     * @param depth     calculation depth - analyzing all moves up to that depth
     * @return          move-info : position of stone and index of that move in the list of possible moves
     */
    MoveInfo computeNextMove( const Reversi::Stone stone, const int depth );

    /*! @brief cancel the calculation of the next move
     *
     */
    void stop()
    { m_stopCalculation = true; }

protected:
    /*! qbrief get char to display for certain stone
     *
     * @param stone     stone to show
     * @return          character tp display
     */
    static int stone2Char( const Reversi::Stone stone );

    /*! @brief get current score of the game regarding a stone
     *
     * @param stone     stone to check
     * @return          score
     */
    int      getScore( const Reversi::Stone stone ) const;

    /*! @brief get max score
     *
     * @param stone     stone to check
     * @param depth     max depth regarding analyzation
     * @param alpha     player-score regarding stone
     * @param beta      score regarding opponent
     * @return
     */
    int      maxScore( const Reversi::Stone stone, const int depth, int alpha, const int beta );

    /*! @brief get min score
     *
     * @param stone     stone to check
     * @param depth     max depth regarding analyzation
     * @param alpha     player-score regarding stone
     * @param beta      score regarding opponent
     * @return
     */
    int      minScore( const Reversi::Stone stone, const int depth, const int alpha, int beta );

private:

    CursesGrid&         m_gridView;                                     ///< display
    Reversi&            m_reversi;                                      ///< gaming engine
    Pos_Vect            m_curPos{ 0, 0 };                               ///< current position
    int                 m_movesIdx { 0 };                               ///< current index regarding valid moves
    FieldList           m_validMoves {};                                ///< list of valid moves
    FieldList           m_UndoList {};                                  ///< to undo the moves

    std::atomic<bool>   m_stopCalculation { false };                    ///< stop-flag
};

#endif //GAMEHANDLER_H
