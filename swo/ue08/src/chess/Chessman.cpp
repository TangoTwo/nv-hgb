#include "Chessman.hpp"
#include "Chessboard.hpp"

// ----------------------- ~chessman~ ----------------------- //

bool Chessman::is_essential() const
{
    return false;
}

// ----------------------- king ----------------------- //

bool King::is_essential() const
{
    return true;
}

char King::get_representation() const
{
    return 'K';
}

bool King::can_move(const std::pair<Chess::Position, Chess::Position> &move) const
{
    return false;
}

// ----------------------- queen ----------------------- //

char Queen::get_representation() const
{
    return 'Q';
}

bool Queen::can_move(const std::pair<Chess::Position, Chess::Position> &move) const
{
    return false;
}

// ----------------------- bishop ----------------------- //

char Bishop::get_representation() const
{
    return 'B';
}

bool Bishop::can_move(const std::pair<Chess::Position, Chess::Position> &move) const
{
    return false;
}

// ----------------------- rook ----------------------- //

char Rook::get_representation() const
{
    return 'R';
}

bool Rook::can_move(const std::pair<Chess::Position, Chess::Position> &move) const
{
    return false;
}

// ----------------------- knight ----------------------- //

char Knight::get_representation() const
{
    return 'N';
}

bool Knight::can_move(const std::pair<Chess::Position, Chess::Position> &move) const
{
    return false;
}

// ----------------------- pawn ----------------------- //

char Pawn::get_representation() const
{
    return 'P';
}

bool Pawn::can_move(const std::pair<Chess::Position, Chess::Position> &move) const
{
    return false;
}