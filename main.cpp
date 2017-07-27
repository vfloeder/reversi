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

#include <signal.h>
#include <atomic>

#include "FieldValue.h"
#include "TerminalWindow.h"

#include "Reversi.h"
#include "GameHandler.h"

// =====================================================================================================================

int main()
{
    const int t_cols { 50 };
    const int t_rows { 25 };
    const int gridSize { 8 };

    static const std::vector<std::string> helpText {
        { "Simple game of REVERSI" },
        { "" },
        { "The status line gives some information" },
        { "regarding a C(ounter) W(hite score) B(lackscore)" },
        { "" },
        { "Empty fields are marked GREEN" },
        { "BLACK/WHITE fields are occupied"},
        { "" },
        { "Valid moves are marked by their value"},
        { "'0'..'9' or '+' where the selected one" },
        { "is printed RED - the highest score is"},
        { "preselected. "},
        { "" },
        { "Press 'h' for help, 'q' to quit" },
        { "       SPACE to select next move"},
        { "       ENTER to execute a move"},
        { "      'u' to undo a move"},
        {""},
        { " Press ENTER to continue... "}
    };
    TerminalWindow termWin(t_cols, t_rows);
    CursesGrid     gridView(termWin, gridSize);
    Reversi        reversi(gridSize);

    const int emptyField = ACS_BULLET | COLOR_PAIR(1);                              // use this one to mark an empty field
    termWin.setEmptyChar(emptyField);

    GameHandler    game(gridView, reversi);

    // print some status info regarding the game
    auto statusPrint { [&gridView]( int cnt, int wcnt, int bcnt, int value, const std::string& line ) -> void
                       {
                           std::stringstream   sstr;

                           sstr << "C=" << static_cast<char>('0' + ( cnt / 20 ) % 10)
                                << " W=" << std::setw(2) << wcnt << " B="  << std::setw(2)
                                << bcnt << " V=" << std::setw(2) << value
                                << " - " << line;
                           gridView.printStatus(sstr.str());
                       }};

    // add the viewable board to the display, so that it is updated
    termWin.addObject(&gridView);

    int             cnt { 0 };
    bool            abortGame { false };
    Reversi::Stone  thisMove { Reversi::Stone::WhiteStone };                        // start with the WHITE stone

    gridView.printHelp(helpText);
    while( getch() != 10 )
        ;
    gridView.print();

    while( !game.ended() && !abortGame )
    {
        const bool  canMove { game.prepareNextMove(thisMove) };
        bool        wait4Move { true };

        if( game.ended() )
        {
            statusPrint(++cnt, game.getWhiteStones(), game.getBlackStones(), 0, "Game Over!");

            while( 'q' != getch() )
                ;
            abortGame = true;
            wait4Move = false;
        }
        else  if( false == canMove )                                                // continue with game, but no move
        {                                                                           //      for current player
            if( Reversi::Stone::WhiteStone == thisMove )
                statusPrint(++cnt, game.getWhiteStones(), game.getBlackStones(), 0, "WHITE No Move press <CR>");
            else
                statusPrint(++cnt, game.getWhiteStones(), game.getBlackStones(), 0, "BLACK No Move press <CR>");

            while( 10 != getch() )                                                  // wait for player to recognize that
                ;

            thisMove = Reversi::otherColor(thisMove);                               // continue afterwards
            wait4Move = false;                                                      //      no choice!
        }

        if( Reversi::Stone::WhiteStone == thisMove )
        {
            GameHandler::MoveInfo inf = game.computeNextMove(thisMove, 4);

            game.prepareNextMove(thisMove);

            if( inf.idx >= 0 )                                                      // if possible move
            {
                game.selectValidMove(thisMove, inf.idx);

                move(20, 0);
                clrtoeol();

                move(20, 0);
                std::stringstream sstr;

                sstr << " -- (" << std::setw(2) << inf.pos.getX() << ", " << std::setw(2) << inf.pos.getY() << ")";
                addstr(sstr.str().c_str());
            }
        }

        // possible to move, let the player choose from the list of valid moves
        while( wait4Move )                                                          // wait for player to select a position
        {
            switch( getch() )
            {
            case 'h' :
                gridView.printHelp(helpText);
                while( getch() != 10 )
                    ;
                gridView.print();
                break;
            case 'u' :                                                              // undo last move
                wait4Move = false;

                if( game.undoMove() )                                               // if undo possible
                {
                    thisMove = Reversi::otherColor(thisMove);                       // next move by opponent
                }

                break;

            case 'q' :                                                              // end the game
                wait4Move = false;
                abortGame = true;
                break;

            case ' ' :                                                              // go to the next valid position
                game.selectNextValidMove(thisMove);
                break;

            case 10 :                                                               // select move
                game.makeMove(thisMove);
                wait4Move = false;

                thisMove = Reversi::otherColor(thisMove);                           // next move by opponent

                break;
            }

            const int   possibleMoves { game.getPossibleFlips() };
            const char* hintText      { thisMove == Reversi::Stone::WhiteStone ? "Move for WHITE..." : "Move for BLACK..." };

            statusPrint(++cnt, game.getWhiteStones(), game.getBlackStones(), possibleMoves, hintText);

        }
    }

    return 0;

}

