#include "Chessman.hpp"
#include <functional>

namespace Chess {

/**
 * Helper class
 */
class Distance {
public: // methods
    explicit Distance(const Move &move)
    {
        const auto &from = move.first;
        const auto &to = move.second;

        x = std::tolower(to.second) - std::tolower(from.second);
        y = to.first - from.first;
    }
public: // members
    int x;
    int y;
};

using TileInterpolator = std::function<Position (int)>;

/**
 * This is absolute magic.
 * I tried explaining this to myself as if I had not written
 * this piece of code and I failed flawlessly. Think of it as
 * "an iterable interpolation for a given difference between
 * to chessmen (positions)".
 * @param pos The position from which to start.
 * @param delta The difference to interpolate.
 * @return "An iterable interpolation".
 */
static TileInterpolator interpolate(Position pos, const Distance &delta)
{
    // .
    auto map = [](Position start, Distance delta, int i) -> Position {
        // the factors providing a "mapping direction"
        int hor_fact = delta.x == 0 ? 0 : delta.x / std::abs(delta.x);
        int vert_fact = delta.y == 0 ? 0 :  delta.y / std::abs(delta.y);

        int row = start.first + (delta.y == 0 ? 0 : i) * vert_fact;
        int col = start.second + (delta.x == 0 ? 0 : i) * hor_fact;
        return { static_cast<unsigned>(row), static_cast<char>(col) };
    };
    return std::bind(map, pos, delta, std::placeholders::_1);
}

/**
 * @return The value of the parameter that is not 0.
 */
static int not0(int i1, int i2) {
    if (i1 != 0 && i2 != 0) {
        throw std::invalid_argument("Both arguments where != 0");
    }
    return i1 == 0 ? i2 : i1;
}

// ----------------------- ~chessman~ ----------------------- //

bool Chessman::is_essential() const
{
    return false;
}

void Chessman::render(std::ostream &os) const
{
    auto transform = (color == Color::BLACK) ? toupper : tolower;
    os << static_cast<char>(transform(get_representation()));
}

// ----------------------- king ----------------------- //

bool King::is_essential() const
{
    return true;
}

char King::get_representation() const
{
    return 'k';
}

bool King::can_move(const Board &board, const Move &move) const
{
    Distance d(move);
    // the king must not move more than 1 tile at a time!
    return std::abs(d.x) <= 1 && std::abs(d.y) <= 1;
}

// ----------------------- queen ----------------------- //

char Queen::get_representation() const
{
    return 'q';
}

bool Queen::can_move(const Board &board, const Move &move) const
{
    Distance d(move);
    // the queen must only move horizontally, vertically or diagonally
    bool pattern_correct =  std::abs(d.x) == std::abs(d.y) || d.y == 0 || d.x == 0;

    // initialized with pattern_correct to prevent useless
    // board traversals because the move will not become
    // valid if the pattern is incorrect in the first place.
    bool no_cm_inbetween = pattern_correct;

    // for all steps in the move vector,
    // verify that there is no chessman sitting
    // at the currently checked tile
    TileInterpolator tile_at = interpolate(move.first, d);
    int i = std::abs(d.x) - 1;
    while (i > 0 && no_cm_inbetween) {
        Position potential_boi = tile_at(i);
        no_cm_inbetween = board.get_chessman_at(potential_boi) == nullptr;
        --i;
    }
    return pattern_correct && no_cm_inbetween;
}

// ----------------------- bishop ----------------------- //

char Bishop::get_representation() const
{
    return 'b';
}

bool Bishop::can_move(const Chess::Board &board, const Move &move) const
{
    Distance d(move);
    // a bishop may only move diagonally!
    bool pattern_correct = std::abs(d.x) == std::abs(d.y);

    bool no_cm_inbetween = pattern_correct;
    TileInterpolator tile_at = interpolate(move.first, d);
    int i = std::abs(d.x) - 1;
    while (i > 0 && no_cm_inbetween) {
        Position potential_boi = tile_at(i);
        no_cm_inbetween = board.get_chessman_at(potential_boi) == nullptr;
        --i;
    }

    return pattern_correct && no_cm_inbetween;
}

// ----------------------- rook ----------------------- //

char Rook::get_representation() const
{
    return 'r';
}

bool Rook::can_move(const Chess::Board &board, const Move &move) const
{
    Distance d(move);
    // a rook can only move along the axes!
    bool pattern_correct = d.x == 0 || d.y == 0;
    bool no_cm_inbetween = pattern_correct;
    if (pattern_correct) {
        TileInterpolator map = interpolate(move.first, d);
        int i = std::abs(not0(d.x, d.y)) - 1;
        while (i != 0 && no_cm_inbetween) {
            Position potential_boi = map(i);
            no_cm_inbetween = board.get_chessman_at(potential_boi) == nullptr;
            --i;
        }
    }

    return pattern_correct && no_cm_inbetween;
}

// ----------------------- knight ----------------------- //

char Knight::get_representation() const
{
    return 'n';
}

bool Knight::can_move(const Chess::Board &board, const Move &move) const
{
    Distance d(move);
    // A knight can only ove 2 tiles in one and 1 tile in the
    // other dimension.
    return (std::abs(d.x) == 2 && std::abs(d.y) == 1) ||
            (std::abs(d.x) == 1 && std::abs(d.y) == 2);
}

// ----------------------- pawn ----------------------- //

char Pawn::get_representation() const
{
    return 'p';
}

bool Pawn::can_move(const Chess::Board &board, const Move &move) const
{
    Distance d(move);

    int steps = board.is_first_move() ? 2 : 1;

    // black may only move down
    bool black_down = color == Color::BLACK && (0 < d.y && d.y <= steps);

    // white may only move up
    bool white_up = color == Color::WHITE && (-steps <= d.y && d.y < 0);

    //
    bool straight = d.x == 0;

    // may only move diagonally if beating another boi
    bool diagonal_beat = std::abs(d.x) == 1 && std::abs(d.y) == 1 &&
            board.get_chessman_at(move.second) != nullptr;

    return (black_down || white_up) && (straight || diagonal_beat);
}

}