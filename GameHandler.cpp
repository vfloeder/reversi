//
// Copyright (c) 2017 Volker Floeder
// All rights reserved.
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

bool GameHandler::prepareNextMove( const Reversi::Stone stone, const bool view )
{
    const bool reverse { Reversi::Stone::WhiteStone == stone };

    m_validMoves = m_reversi.getValidMoves(stone);                                  // get list of allowed moves

    if( view )
    {
        m_gridView.markCells(m_validMoves, reverse);                                // mark the fields
        m_gridView.print();
    }

    if( m_validMoves.size())                                                        // if move is possible
    {
        m_movesIdx = m_validMoves.getBestPos();                                     // get entry with maximum flips

        m_curPos = m_validMoves[m_movesIdx].getFieldPosition();                     // set cursor to max. flips

        if( view )
            m_gridView.markCell(m_curPos, reverse);

        return true;
    }

    return false;
}

// ---------------------------------------------------------------------------------------------------------------------

void GameHandler::selectNextValidMove( const Reversi::Stone stone, const bool view )
{
    const bool reverse { stone == Reversi::Stone::WhiteStone ? true : false };

    if( view )
        m_gridView.unmarkCell(m_validMoves[m_movesIdx].getFieldPosition(), reverse);

    m_movesIdx = ( m_movesIdx + 1 ) % m_validMoves.size();
    m_curPos   = m_validMoves[m_movesIdx].getFieldPosition();
    if( view )
        m_gridView.markCell(m_curPos, reverse);
}

// ---------------------------------------------------------------------------------------------------------------------

void GameHandler::selectValidMove( const Reversi::Stone stone, const int idx )
{
    const bool reverse { stone == Reversi::Stone::WhiteStone ? true : false };

    m_gridView.unmarkCell(m_validMoves[m_movesIdx].getFieldPosition(), reverse);

    m_movesIdx = idx;

    m_curPos   = m_validMoves[m_movesIdx].getFieldPosition();
    m_gridView.markCell(m_curPos, reverse);
}

// ---------------------------------------------------------------------------------------------------------------------

void GameHandler::makeMove( const Reversi::Stone stone, const bool view )
{
    if( view ) m_gridView.unmarkCells(m_validMoves);                                // unmark since decision is made

    m_reversi.setStone(m_curPos, stone);                                            // set the stone
    if( view ) m_gridView.setChar(m_curPos, stone2Char(stone));                     // visualize it

    // flip all "won" stones
    for( const auto& j : m_validMoves[m_movesIdx] )
    {
        m_reversi.flipStone(j);                                                     // change game-board
        if( view ) m_gridView.setChar(j, stone2Char(m_reversi.peekField(j)));       //      and display
    }

    // store the undo information for this move
    FieldValue  storedMove { m_validMoves[m_movesIdx] };
    storedMove.setFieldPosition(m_curPos);
    m_UndoList.push_back(storedMove);
}

// ---------------------------------------------------------------------------------------------------------------------

bool GameHandler::undoMove( const bool view )
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
    m_reversi.removeStone(undo.getFieldPosition());

    return true;
}

// ---------------------------------------------------------------------------------------------------------------------

int GameHandler::getPossibleFlips()
{
    int idx { m_validMoves.getBestPos() };                                          // get entry with maximum flips

    return idx >= 0 ? m_validMoves[idx].getValue() : 0;
}

// ---------------------------------------------------------------------------------------------------------------------

int GameHandler::stone2Char( const Reversi::Stone stone )
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

int GameHandler::getScore( const Reversi::Stone stone ) const
{
    return Reversi::Stone::WhiteStone == stone
           ? m_reversi.getWhiteNum() - m_reversi.getBlackNum()
           : m_reversi.getBlackNum() - m_reversi.getWhiteNum();
}

// ---------------------------------------------------------------------------------------------------------------------

// currently limited by depth but should rather be limited by time

GameHandler::MoveInfo GameHandler::computeNextMove( const Reversi::Stone stone, const int depth )
{
    MoveInfo    ret { {-1, -1}, -1 };
    int         alpha { -m_reversi.getBoardSize() };
    const int   beta { m_reversi.getBoardSize() };

    m_stopCalculation = false;                                                      // assume to keep working

    prepareNextMove(stone, false);                                                  // prepare game info regarding move

    const int validMoves { static_cast<int>(m_validMoves.size()) };                 // get number of possible moves
    int idx { 0 };

    while( idx < validMoves )                                                       // iterate over them
    {
        if( m_stopCalculation ) break;

        makeMove(stone, false);                                                     // make this move

        const int score { minScore(Reversi::otherColor(stone), depth - 1, alpha, beta) };  // calculate min score

        undoMove(false);                                                            // undo the move
        prepareNextMove(stone, false);                                              // prepare for the next move

        if( score > alpha )
            alpha = score;

        ret.pos = m_validMoves[idx].getFieldPosition();
        ret.idx = idx;

        if( m_reversi.gameOver() )
            break;

        ++idx;
    }

    return ret;
}

// ---------------------------------------------------------------------------------------------------------------------
// heuristic https://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/

int GameHandler::maxScore( const Reversi::Stone stone, const int depth, int alpha, const int beta )
{
    if( m_reversi.gameOver() )
        return getScore(stone);

    if( depth <= 0 )
        return getScore(stone);                                                     // should rather be heuristic

    prepareNextMove(stone, false);

    const int   validMoves { static_cast<int>(m_validMoves.size()) };
    int         idx { 0 };
    int         bestScore { -m_reversi.getBoardSize() };

    while( idx < validMoves )
    {
        if( m_stopCalculation ) break;

        makeMove(stone, false);

        const int score = minScore(Reversi::otherColor(stone), depth - 1, alpha, beta);

        undoMove(false);
        prepareNextMove(stone, false);

        bestScore = std::max(bestScore, score);

        alpha = std::max(alpha, bestScore);

        if( alpha >= beta )
            break;

        selectNextValidMove(stone, false);

        ++idx;
    }
    return bestScore;
}

// ---------------------------------------------------------------------------------------------------------------------

int GameHandler::minScore( const Reversi::Stone stone, const int depth, const int alpha, int beta )
{
    if( m_reversi.gameOver() )
        return getScore(stone);

    if( depth <= 0 )
        return getScore(stone);

    prepareNextMove(stone, false);

    const int   validMoves { static_cast<int>(m_validMoves.size()) };
    int         idx { 0 };
    int         bestScore { m_reversi.getBoardSize() };

    while( idx < validMoves )
    {
        if( m_stopCalculation ) break;

        makeMove(stone, false);

        const int score = maxScore(Reversi::otherColor(stone), depth - 1, alpha, beta);

        undoMove(false);
        prepareNextMove(stone, false);

        bestScore = std::min(bestScore, score);

        beta = std::min(beta, bestScore);

        if( alpha >= beta )
            break;

        selectNextValidMove(stone, false);

        ++idx;
    }
    return bestScore;
}
