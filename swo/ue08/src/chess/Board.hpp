#pragma once

#include <map>
#include <memory>
#include <vector>
#include "Game.cpp"

class Chessman;

class Chessboard
{
public: // typedefs
    using ChessmanPtr = std::shared_ptr<Chessman>;

public: // methods
    explicit Chessboard(int side_length);



private: // members
    std::vector<std::vector<ChessmanPtr>> _board;
    bool _is_over {false};
    ChessmanPtr _currently_held_chessman;
};


