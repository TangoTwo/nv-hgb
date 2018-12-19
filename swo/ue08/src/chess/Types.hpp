#pragma once

#include <functional>
#include <map>
#include <memory>

/*
 * This file contains forward declarations for types that are important for all classes.
 * That way cyclic dependencies can be avoided.
 */

namespace Chess {

class Game;

class Board;

class Chessman;

using ChessmanPtr = std::shared_ptr<Chessman>;

/**
 * Represents a tile on the chess board.
 */
using Position = std::pair<unsigned int, char>;

std::istream &operator>>(std::istream &is, Position &pos);

using Move = std::pair<Position, Position>;

/**
 * The characters surrounding the representation
 * of a chess tile / chessman.
 */
using Highlight = std::pair<char, char>;

/**
 * Because booleans felt too hacky.
 */
enum class Color
{
    BLACK, WHITE
};

std::ostream &operator<<(std::ostream &os, Color color);

} // namespace Chess