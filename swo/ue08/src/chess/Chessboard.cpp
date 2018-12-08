#include "Chessboard.hpp"

Chessboard::Chessboard(int side_length)
{
    // reserve vector space
}

const bool & Chessboard::is_over() const
{
    return _is_over;
}

const Chessboard::ChessmanPtr Chessboard::get_chessman_at(unsigned int row, char column) const
{
    return get_chessman_at(std::make_pair(row, column));
}

const Chessboard::ChessmanPtr Chessboard::get_chessman_at(const Chess::Position &position) const
{
    return Chessboard::ChessmanPtr(); // TODO
}
