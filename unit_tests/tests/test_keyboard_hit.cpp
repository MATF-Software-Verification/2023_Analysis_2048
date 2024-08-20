#define CATCH_CONFIG_MAIN
#include <unistd.h>
#include "catch.hpp"
#include "../../2048/app/Game.hpp"

class StdinRestorer{
public:
    StdinRestorer(){
        saved_stdin = dup(STDIN_FILENO);
    }
    ~StdinRestorer(){
        if (saved_stdin != -1){
            dup2(saved_stdin,STDIN_FILENO);
            close(saved_stdin);
        }
    }
private:
    int saved_stdin;
};

int simulateInput(const char* input){
    Game game;
    game.init();
    // pipe to simulate stdin
    int pipefd[2];
    pipe(pipefd);

    write(pipefd[1], input, 2);
    close(pipefd[1]);

    StdinRestorer stdin_restorer;

    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);

    game.update();

    return game.getKey();
}

TEST_CASE("Test keyboard input 'w'") {
    int key = simulateInput("w");

    REQUIRE(key == int('w'));
}

TEST_CASE("Test keyboard input 'a'") {
    int key = simulateInput("a");

    REQUIRE(key == int('a'));
}

TEST_CASE("Test keyboard input 's'") {
    int key = simulateInput("s");

    REQUIRE(key == int('s'));
}

TEST_CASE("Test keyboard input 'd'") {
    int key = simulateInput("d");

    REQUIRE(key == int('d'));
}

TEST_CASE("Test keyboard input 'h'") {
    int key = simulateInput("h");

    REQUIRE(key == int('h'));
}

TEST_CASE("Test keyboard input 'j'") {
    int key = simulateInput("j");

    REQUIRE(key == int('j'));
}

TEST_CASE("Test keyboard input 'k'") {
    int key = simulateInput("k");

    REQUIRE(key == int('k'));
}

TEST_CASE("Test keyboard input 'l'") {
    int key = simulateInput("l");

    REQUIRE(key == int('l'));
}
