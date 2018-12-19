#include "Game.hpp"

#include <functional>

namespace Chess {

void Game::next_move(std::istream &is, std::ostream &os)
{
    auto read_from_is = [&is]() {
        Position x;
        is >> x;
        return x;
    };
    next_move(read_from_is, os);
}

void Game::next_move(std::function<Position()> get_pos, std::ostream &os)
{
    _board.render(os);

    // ========= picking ========= //
    _prompt("Chose a chessman to move (row col)", os);

    Position from = get_pos();

    while (!_board.is_pickable(from)) {
        os << "Invalid chessman, try another one!" << std::endl;
        _prompt("Chose a chessman to move (row col)", os);
        from = get_pos();
    }
    _board.pick(from);

    _board.render(os);

    // ========= dropping ========= //
    _prompt("Move selected chessman to", os);

    Position to = get_pos();

    while (!_board.is_current_chessman_droppable_at(to)) {
        os << "Can not move selected chessman to specified position!" << std::endl;
        _prompt("Move selected chessman to", os);
        to = get_pos();
    }
    _board.drop(to);

    if (!is_over()) { // dont flip after game is over so the winner can be queried!
        _current_player = (get_current_player() == Color::WHITE ? Color::BLACK : Color::WHITE);
    } else {
        os << "Player " << get_current_player() << " won!" << std::endl;
    }
}

bool Game::is_over() const
{
    return _board.essential_lost();
}

void Game::_prompt(const std::string &prompt, std::ostream &os)
{
    os << get_current_player() << ' ' << prompt << "> " << std::flush;
}

Color Game::get_current_player() const
{
    return _current_player;
}

}