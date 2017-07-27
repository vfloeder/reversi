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

#include "GameHandler.h"

// =====================================================================================================================

GameHandler::GameHandler( CursesGrid& gridView, Reversi& reversi )
    : m_gridView { gridView }
    , m_reversi { reversi }
{
    // initialize display regarding initial state of the game board
    for( auto i = m_reversi.begin(); i != m_reversi.end(); ++i )
    {
        const Reversi::Stone stone { *i };
        const Pos_Vect       pos { i.getPosition() };

        m_gridView.setChar(pos, stone2Char(stone));
    }
    m_gridView.setCursor(m_curPos);
}

// ---------------------------------------------------------------------------------------------------------------------

bool GameHandler::ended() const
{
    return m_reversi.gameOver();
}

// ---------------------------------------------------------------------------------------------------------------------

int GameHandler::getWhiteStones() const
{
    return m_reversi.getWhiteNum();
}

// ---------------------------------------------------------------------------------------------------------------------

int GameHandler::getBlackStones() const
{
    return m_reversi.getBlackNum();
}

// ---------------------------------------------------------------------------------------------------------------------

bool GameHandler::prepareNextMove( Reversi::Stone thisMove, bool view )
{
    const bool reverse { Reversi::Stone::WhiteStone == thisMove };

    m_validMoves = m_reversi.getValidMoves(thisMove);                               // get list of allowed moves

    if( view )
    {
        m_gridView.markCells(m_validMoves, reverse);                                // mark the fields
        m_gridView.print();
    }

    if( m_validMoves.size())                                                        // if move is possible
    {
        m_movesIdx = m_validMoves.getBestPos();                                     // get entry with maximum flips

        m_curPos = m_validMoves[m_movesIdx].getPosition();                          // set cursor to max. flips

        if( view ) m_gridView.markField(m_curPos, reverse);

        return true;
    }

    return false;
}

// ---------------------------------------------------------------------------------------------------------------------

void GameHandler::selectNextValidMove( Reversi::Stone stone, bool view )
{
    const bool reverse { stone == Reversi::Stone::WhiteStone ? true : false };

    if( view ) m_gridView.unmarkField(m_validMoves[m_movesIdx].getPosition(), reverse);

    m_movesIdx = ( m_movesIdx + 1 ) % m_validMoves.size();
    m_curPos   = m_validMoves[m_movesIdx].getPosition();
    if( view ) m_gridView.markField(m_curPos, reverse);
}

// ---------------------------------------------------------------------------------------------------------------------

void GameHandler::selectValidMove( Reversi::Stone stone, int idx )
{
    const bool reverse { stone == Reversi::Stone::WhiteStone ? true : false };

    m_gridView.unmarkField(m_validMoves[m_movesIdx].getPosition(), reverse);

    m_movesIdx = idx;

    m_curPos   = m_validMoves[m_movesIdx].getPosition();
    m_gridView.markField(m_curPos, reverse);
}

// ---------------------------------------------------------------------------------------------------------------------

void GameHandler::makeMove( Reversi::Stone thisMove, bool view )
{
    if( view ) m_gridView.unmarkCells(m_validMoves);                                // unmark since decision is made

    m_reversi.setStone(m_curPos, thisMove);                                         // set the stone
    if( view ) m_gridView.setChar(m_curPos, stone2Char(thisMove));                  // visualize it

    // flip all "won" stones
    for( const auto& j : m_validMoves[m_movesIdx] )
    {
        m_reversi.flipStone(j);                                                     // change game-board
        if( view ) m_gridView.setChar(j, stone2Char(m_reversi.peekField(j)));       //      and display
    }

    // store the undo information for this move
    FieldValue  storedMove { m_validMoves[m_movesIdx] };
    storedMove.setPosition(m_curPos);
    m_UndoList.push_back(storedMove);
}

// ---------------------------------------------------------------------------------------------------------------------

bool GameHandler::undoMove( bool view )
{
    if( !m_UndoList.size() ) return false;

    if( view ) m_gridView.unmarkCells(m_validMoves);                                // unmark since decision is made

    FieldValue undo = m_UndoList.back();
    m_UndoList.pop_back();

    for( const auto& i : undo )
    {
        m_reversi.flipStone(i);
        if( view ) m_gridView.setChar(i, stone2Char(m_reversi.peekField(i)));
    }
    m_reversi.removeStone(undo.getPosition());

    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

int GameHandler::getPossibleFlips()
{
    int idx { m_validMoves.getBestPos() };                                          // get entry with maximum flips

    return m_validMoves[idx].getValue();
}

// ---------------------------------------------------------------------------------------------------------------------

int GameHandler::stone2Char( Reversi::Stone stone )
{
    switch( stone )
    {
    case Reversi::Stone::BlackStone : return ' ';
    case Reversi::Stone::NoStone    : return ACS_BULLET | COLOR_PAIR(1);
    case Reversi::Stone::WhiteStone : return ' ' | A_REVERSE;
    }
}

// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------------------------------

int GameHandler::getScore( Reversi::Stone stone )
{
    return Reversi::Stone::WhiteStone == stone
           ? m_reversi.getWhiteNum() - m_reversi.getBlackNum()
           : m_reversi.getBlackNum() - m_reversi.getWhiteNum();
}

// ---------------------------------------------------------------------------------------------------------------------

// currently limited by depth but should rather be limited by time

GameHandler::MoveInfo GameHandler::computeNextMove( Reversi::Stone stone, int depth )
{
    MoveInfo    ret { {-1, -1}, -1 };
    int         alpha { -1000 };
    const int   beta { 1000 };

    prepareNextMove(stone, false);

    const int validMoves { static_cast<int>(m_validMoves.size()) };
    int idx { 0 };

    while( idx < validMoves )
    {
        makeMove(stone, false);

        const int score { minScore(stone, depth - 1, alpha, beta) };

        undoMove(false);
        prepareNextMove(stone, false);

        if( score > alpha )
            alpha = score;

        ret.pos = m_validMoves[idx].getPosition();
        ret.idx = idx;

        if( m_reversi.gameOver() )
            break;

        ++idx;
    }

    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------
// heuristic https://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/

int GameHandler::maxScore( Reversi::Stone stone, int depth, int alpha, int beta )
{
    if( m_reversi.gameOver() )
        return getScore(stone);

    if( depth <= 0 )
        return getScore(stone);                                                     // should rather be heuristic

    prepareNextMove(stone, false);

    const int validMoves { static_cast<int>(m_validMoves.size()) };
    int       idx { 0 };

    while( idx < validMoves )
    {
        makeMove(stone, false);

        const int score = minScore(stone, depth - 1, alpha, beta);

        undoMove(false);
        prepareNextMove(stone, false);

        alpha = std::max(alpha, score);

        if( alpha >= beta )
            return beta;

        selectNextValidMove(stone, false);

        ++idx;
    }
    return alpha;
}

// ---------------------------------------------------------------------------------------------------------------------

int GameHandler::minScore( Reversi::Stone stone, int depth, int alpha, int beta )
{
    if( m_reversi.gameOver() )
        return getScore(stone);

    if( depth <= 0 )
        return getScore(stone);

    prepareNextMove(stone, false);

    const int validMoves { static_cast<int>(m_validMoves.size()) };
    int       idx { 0 };

    while( idx < validMoves )
    {
        makeMove(stone, false);

        const int score = maxScore(stone, depth - 1, alpha, beta);

        undoMove(false);
        prepareNextMove(stone, false);

        beta = std::min(beta, score);

        if( alpha >= beta )
            return alpha;

        selectNextValidMove(stone, false);

        ++idx;
    }
    return beta;
}
