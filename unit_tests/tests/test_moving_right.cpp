#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../../2048/app/Game.hpp"
#include "../../2048/app/KeyPushManager.hpp"
#include "../../2048/app/Kbhit.h"
#include "../../2048/app/Config.hpp"

//Game.cpp -> update(), KeyPushManager -> waitKeyPush(), Kbhit -> getch(), KeyBoardManager -> onKeyboardHit(), moveRight(), moveRight(), moveUp(), moveRight()

TEST_CASE("Moving right (empty rows/cols, no merging tiles)"){
    Game game;
    Game::GameBoard initial = {
     {0,2,0,0,0},
     {0,0,0,0,0},
     {0,16,0,4,0},
     {0,0,8,0,0}
    };
    game.setGameBoard(initial);
    KeyPushManager key(game);
    key.moveRight();
    Game::GameBoard newGb = key.getGameBoard();

    Game::GameBoard expected = {
     {0,0,0,0,2},
     {0,0,0,0,0},
     {0,0,0,16,4},
     {0,0,0,0,8}
    };

    int eq = 1;
    for(int i=0;i<GAMEBOARD_ROWS;i++)
        for (int j=0;j<GAMEBOARD_COLS;j++)
            if (newGb.at(i).at(j)!=expected.at(i).at(j)){
                eq=0;
                break;
            }

    REQUIRE(eq==1);
}

TEST_CASE("Moving right (possible merges)"){
    Game game;
    Game::GameBoard initial = {
     {0,2,0,0,0},
     {0,0,2,0,2},
     {0,2,0,4,0},
     {0,0,4,4,0}
    };
    game.setGameBoard(initial);
    KeyPushManager key(game);
    key.moveRight();
    Game::GameBoard newGb = key.getGameBoard();

    Game::GameBoard expected = {
     {0,0,0,0,2},
     {0,0,0,0,4},
     {0,0,0,2,4},
     {0,0,0,0,8}
    };

    int eq = 1;
    for(int i=0;i<GAMEBOARD_ROWS;i++)
        for (int j=0;j<GAMEBOARD_COLS;j++)
            if (newGb.at(i).at(j)!=expected.at(i).at(j)){
                eq=0;
                break;
            }

    REQUIRE(eq==1);
}

TEST_CASE("Moving right (overlaping merges)"){
    Game game;
    Game::GameBoard initial = {
     {0,16,0,2,0},
     {2,2,4,4,0},
     {0,4,4,0,4},
     {0,0,8,4,4}
    };
    game.setGameBoard(initial);
    KeyPushManager key(game);
    key.moveRight();
    Game::GameBoard newGb = key.getGameBoard();

    Game::GameBoard expected = {
     {0,0,0,16,2},
     {0,0,0,8,4},
     {0,0,0,8,4},
     {0,0,0,0,16}
    };

    int eq = 1;
    for(int i=0;i<GAMEBOARD_ROWS;i++)
        for (int j=0;j<GAMEBOARD_COLS;j++)
            if (newGb.at(i).at(j)!=expected.at(i).at(j)){
                eq=0;
                break;
            }

    REQUIRE(eq==1);
}

TEST_CASE("Moving right (no possible moves)"){
    Game game;
    Game::GameBoard initial = {
     {2,4,8,2,4},
     {0,16,2,256,16},
     {0,0,8,32,2},
     {16,8,2,16,128}
    };
    game.setGameBoard(initial);
    KeyPushManager key(game);
    key.moveRight();
    Game::GameBoard newGb = key.getGameBoard();

    Game::GameBoard expected = {
     {2,4,8,2,4},
     {0,16,2,256,16},
     {0,0,8,32,2},
     {16,8,2,16,128}
    };

    int eq = 1;
    for(int i=0;i<GAMEBOARD_ROWS;i++)
        for (int j=0;j<GAMEBOARD_COLS;j++)
            if (newGb.at(i).at(j)!=expected.at(i).at(j)){
                eq=0;
                break;
            }

    REQUIRE(eq==1);
}
