#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "../../2048/app/Game.hpp"

//Game.cpp -> checkFinish()

  /* Igra je zavrsena ako je tabla popunjena i nema mogucih koraka
     * I slucaj: tabla nije popunjena -> nije zavrseno
     * II slucaj: tabla jeste popunjena, ali postoji moguci korak -> nije zavrseno
     * III slucaj: tabla jeste popunjena, ali ne postoji moguci korak -> nije zavrseno
     * */

TEST_CASE("Testing if the game ends with an empty feild"){
    Game game;
    game.init();

    Game::GameBoard gb = {
        {2,4,8,2,32},
        {4,8,2,256,16},
        {32,4,0,2,32},
        {16,8,2,16,128}
    };
    game.setGameBoard(gb);
    game.checkFinish();

    REQUIRE(game.isFinish()==false);
}

TEST_CASE("Testing if the game ends with a possible move 1"){
    Game game;
    game.init();

    Game::GameBoard gb = {
        {2,4,8,2,32},
        {4,8,2,256,16},
        {32,4,4,2,32},
        {16,8,2,16,128}
    };
    game.setGameBoard(gb);
    game.checkFinish();

    REQUIRE(game.isFinish()==false);
}

TEST_CASE("Testing if the game ends with a possible move 2"){
    Game game;
    game.init();

    Game::GameBoard gb = {
        {2,4,8,256,32},
        {4,8,2,256,16},
        {32,4,16,2,32},
        {16,8,2,16,128}
    };
    game.setGameBoard(gb);
    game.checkFinish();

    REQUIRE(game.isFinish()==false);
}

TEST_CASE("Testing if the game ends with no possible moves"){
    Game game;
    game.init();
    Game::GameBoard gb = {
        {2,4,8,2,32},
        {4,8,2,256,16},
        {32,4,8,2,32},
        {16,8,2,16,128}
    };
    game.setGameBoard(gb);
    game.checkFinish();

    REQUIRE(game.isFinish()==true);
}
