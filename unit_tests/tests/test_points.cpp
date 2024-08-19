#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../../2048/app/Game.hpp"
#include "../../2048/app/KeyPushManager.hpp"
#include "../../2048/app/Config.hpp"

int getPoints(Game::GameBoard &gb, char k){
    Game game;
    game.setGameBoard(gb);
    game.setPoint(10);
    KeyPushManager key(game);
    key.setMKey(int(k));
    key.onKeyboardHitRefactored();
    int points = game.getPoint();

    return points;
}

TEST_CASE("Test sum of points on initial table"){
    Game game;
    game.init();
    Game::GameBoard gb = game.getGameBoard();

    REQUIRE(game.getPoint() == 0);
}

TEST_CASE("Test sum of points when there are no merges"){
    Game game;
    Game::GameBoard gb = {
     {0,2,0,0,0},
     {0,0,0,0,0},
     {0,16,0,4,0},
     {0,0,8,0,0}
    };

    int points = getPoints(gb,'d');

    REQUIRE(points == 10);
}

TEST_CASE("Test sum of points when there are merges"){
    Game game;
    Game::GameBoard gb = {
     {0,0,0,0,0},
     {0,2,0,4,2},
     {0,2,0,0,0},
     {0,0,0,4,0}
    };

    int points = getPoints(gb,'s');

    REQUIRE(points == 16);
}

TEST_CASE("Test sum of points when there are overlaping merges"){
    Game game;
    Game::GameBoard gb = {
     {2,0,0,2,0},
     {2,0,4,0,4},
     {4,0,4,0,4},
     {4,0,8,4,4}
    };

    int points = getPoints(gb,'w');

    REQUIRE(points == 32);
}

TEST_CASE("Test sum of points when there are different numbers"){
    Game game;
    Game::GameBoard gb = {
     {0,0,16,4,16},
     {0,32,32,64,0},
     {0,0,128,128},
     {0,0,1024,0,1024}
    };

    int points = getPoints(gb,'a');

    REQUIRE(points == 1258);
}
