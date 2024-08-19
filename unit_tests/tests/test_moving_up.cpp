#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../../2048/app/Game.hpp"
#include "../../2048/app/KeyPushManager.hpp"
#include "../../2048/app/Config.hpp"

int movingUp(Game::GameBoard &i, Game::GameBoard &e){
    Game game;

    game.setGameBoard(i);
    KeyPushManager key(game);
    key.setMKey(int('w'));
    key.onKeyboardHit();
    Game::GameBoard newGb = key.getGameBoard();

    int eq = 1;
    for(int i=0;i<GAMEBOARD_ROWS;i++)
        for (int j=0;j<GAMEBOARD_COLS;j++)
            if (newGb.at(i).at(j)!=e.at(i).at(j)){
                eq=0;
                break;
            }
    return eq;
}

TEST_CASE("Moving up (empty rows/cols, no merging tiles)"){
    Game game;
    Game::GameBoard initial = {
     {0,0,0,0,0},
     {0,2,0,0,0},
     {0,0,0,4,0},
     {0,16,8,0,0}
    };

    Game::GameBoard expected = {
     {0,2,8,4,0},
     {0,16,0,0,0},
     {0,0,0,0,0},
     {0,0,0,0,0}
    };

    int eq = movingUp(initial, expected);

    REQUIRE(eq==1);
}

TEST_CASE("Moving up (possible merges)"){
    Game game;
    Game::GameBoard initial = {
     {0,0,0,0,0},
     {0,2,0,4,2},
     {0,2,0,0,0},
     {0,0,0,4,0}
    };

    Game::GameBoard expected = {
     {0,4,0,8,2},
     {0,0,0,0,0},
     {0,0,0,0,0},
     {0,0,0,0,0}
    };

    int eq = movingUp(initial, expected);

    REQUIRE(eq==1);
}

TEST_CASE("Moving up (overlaping merges)"){
    Game game;
    Game::GameBoard initial = {
     {2,0,0,2,0},
     {2,0,4,0,4},
     {4,0,4,0,4},
     {4,0,8,4,4}
    };

    Game::GameBoard expected = {
     {4,0,16,2,4},
     {8,0,0,4,8},
     {0,0,0,0,0},
     {0,0,0,0,0}
    };

    int eq = movingUp(initial, expected);

    REQUIRE(eq==1);
}

TEST_CASE("Moving up (no possible moves)"){
    Game game;
    Game::GameBoard initial = {
     {2,4,8,2,32},
     {4,8,2,256,16},
     {32,4,0,2,32},
     {16,8,0,16,0}
    };

    Game::GameBoard expected = {
     {2,4,8,2,32},
     {4,8,2,256,16},
     {32,4,0,2,32},
     {16,8,0,16,0}
    };

    int eq = movingUp(initial, expected);

    REQUIRE(eq==1);
}
