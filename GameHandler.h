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

class GameHandler
{
public:
    // info to be returned by the next computed move
    struct MoveInfo {
        Pos_Vect pos;                                                               // position for stone
        int      idx;                                                               // index of move in the list of
                                                                                    //      valids moves
    };
    GameHandler( CursesGrid&  gridView,  Reversi& reversi );

    // game ended?
    bool ended() const;

    // get current number of white stones
    int getWhiteStones() const;

    // get number of black stones
    int getBlackStones() const;

    // prepare for the next move (calculate possibilities...)
    bool prepareNextMove( Reversi::Stone thisMove, bool view = true );

    // move cursor (selection) to the next possibility
    void selectNextValidMove( Reversi::Stone thisMove, bool view = true );

    // set to dedicated position - used for computed moves
    void selectValidMove( Reversi::Stone stone, int idx );

    // select a cursor-position, making that move
    void makeMove( Reversi::Stone  thisMove, bool view = true );

    // undo the last move
    bool undoMove( bool view = true );

    // get possible flips for a position
    int getPossibleFlips();

    // compute a "good" next move by analysing all possibilities down to a certain depth
    MoveInfo computeNextMove( Reversi::Stone stone, int depth );

    void stop()
    { m_stopCalculation = true; }

protected:
    // get char to display for certain stone
    static int stone2Char( Reversi::Stone stone );

    int      getScore( Reversi::Stone stone );

    int      maxScore( Reversi::Stone stone, int depth, int alpha, int beta );
    int      minScore( Reversi::Stone stone, int depth, int alpha, int beta );

private:

    CursesGrid&  m_gridView;                                                    // display
    Reversi&     m_reversi;                                                     // gaming engine
    Pos_Vect     m_curPos{ 0, 0 };                                              // current position
    int          m_movesIdx { 0 };                                              // current index regarding valid moves
    FieldList    m_validMoves {};                                               // list of valid moves
    FieldList    m_UndoList {};                                                 // to undo the moves

    std::atomic<bool>           m_stopCalculation { false };
};

#endif //GAMEHANDLER_H
