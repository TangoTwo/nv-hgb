#pragma once

#include <iostream>
#include "Types.hpp"
#include "Board.hpp"

namespace Chess {

class Game final
{
public: // methods

    /**
     * Initializes a game by setting up a chessboard with chessmen.
     */
    explicit Game(unsigned side_length = 8)
        : _board{*this, side_length}
    {}

    /**
     * Reads a move from the supplied input stream. if the move is valid,
     * the game is advanced, otherwise the stream is read until it yields
     * a valid move.
     * @param is The input stream to read the new move from.
     * @param os The output stream to write the new state of the game to.
     */
    void next_move(std::istream &is, std::ostream &os);

    /**
     * Calls the supplied function to retrieve a move. if the move is valid,
     * the game is advanced, otherwise the function is recalled until it yields
     * a valid move.
     * @param get_pos A function which returns a move.
     * @param os The output stream to write the new state of the game to.
     */
    void next_move(std::function<Position()> get_pos, std::ostream &os);

    /**
     * @return True if one player has lost an essential chessman, False otherwise.
     */
    bool is_over() const;

    /**
     * @return The player who may make the next move in the game.
     */
    Color get_current_player() const;

private: // methods

    /**
     * Writes a prompt to the supplied output stream.
     * @param prompt The message to use for the prompt.
     * @param os The output stream to write the prompt to.
     */
    void _prompt(const std::string &prompt, std::ostream &os);

private: // members

    /**
     * The player "with the action".
     * (https://www.linguee.de/deutsch-englisch/uebersetzung/spieler+der+am+zug+ist.html)
     */
    Color _current_player{Color::BLACK};

    /**
     * The game board associated with this game.
     */
    Board _board;
};

}