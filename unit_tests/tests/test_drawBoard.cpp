#define CATCH_CONFIG_MAIN
#include <iostream>
#include <sstream>
#include "catch.hpp"
#include "../../2048/app/Game.hpp"

std::string stripColor(const std::string& input){
    std::regex colorCode("\x1b\\[[0-9;]*m");
    return std::regex_replace(input,colorCode,"");
}

std::string getOnlyBoard(const std::string &str){
    std::vector<std::string> lines;
    std::stringstream ss(str);
    std::string line;
    while(std::getline(ss,line))
        lines.push_back(line);

    lines.erase(lines.begin());
    lines.pop_back();

    std::ostringstream oss;
    for (const auto& line: lines)
        oss << line << '\n';
    return oss.str();
}

std::string checkGameBoard(Game::GameBoard &gb){
    Game game;

    std::stringstream buffer;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(buffer.rdbuf());

    game.setGameBoard(gb);
    game.drawGameBoard();

    std::cout.rdbuf(oldCoutBuffer);

    std::string output = buffer.str();
    std::string lines = getOnlyBoard(output);

    std::string strippedOfColor = stripColor(lines);

    return strippedOfColor;
}

TEST_CASE("Draw random initial board test"){
    Game game;

    Game::GameBoard gb = {
     {0,0,0,0,0},
     {0,2,0,0,0},
     {0,0,0,0,0},
     {0,0,0,4,0}
    };

    std::string strippedOfColor = checkGameBoard(gb);

    std::string expectedOutput =
    R"(+-----+-----+-----+-----+-----+
|     |     |     |     |     |
+-----+-----+-----+-----+-----+
|     |    2|     |     |     |
+-----+-----+-----+-----+-----+
|     |     |     |     |     |
+-----+-----+-----+-----+-----+
|     |     |     |    4|     |
+-----+-----+-----+-----+-----+
)";

    REQUIRE(strippedOfColor == expectedOutput);
}

TEST_CASE("Draw board test 1"){
    Game game;

    Game::GameBoard gb = {
     {0,2,0,0,0},
     {0,0,4,0,32},
     {256,0,16,64,0},
     {0,8,0,0,128}
    };

    std::string strippedOfColor = checkGameBoard(gb);

    std::string expectedOutput =
    R"(+-----+-----+-----+-----+-----+
|     |    2|     |     |     |
+-----+-----+-----+-----+-----+
|     |     |    4|     |   32|
+-----+-----+-----+-----+-----+
|  256|     |   16|   64|     |
+-----+-----+-----+-----+-----+
|     |    8|     |     |  128|
+-----+-----+-----+-----+-----+
)";

    REQUIRE(strippedOfColor == expectedOutput);
}

TEST_CASE("Draw filled board test"){
    Game game;

    Game::GameBoard gb = {
     {2,4,2,4,16},
     {8,4,32,128,32},
     {16,16,2,64,128},
     {8,4,2,128,256}
    };

    std::string strippedOfColor = checkGameBoard(gb);

    std::string expectedOutput =
    R"(+-----+-----+-----+-----+-----+
|    2|    4|    2|    4|   16|
+-----+-----+-----+-----+-----+
|    8|    4|   32|  128|   32|
+-----+-----+-----+-----+-----+
|   16|   16|    2|   64|  128|
+-----+-----+-----+-----+-----+
|    8|    4|    2|  128|  256|
+-----+-----+-----+-----+-----+
)";

    REQUIRE(strippedOfColor == expectedOutput);
}
