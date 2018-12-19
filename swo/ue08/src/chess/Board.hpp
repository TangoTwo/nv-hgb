#pragma once

#include <vector>
#include "Types.hpp"

namespace Chess {

class Board final
{
public: // methods

    /**
     * Creates a new chess board for a specific game.
     * @param game The game with which this board is associated.
     * @param side_length The desired side length of this board.
     */
    Board(const Game &game, unsigned side_length);

    /**
     * @param pos The indices of the desired tile.
     * @return The chessman at the specified position. If
     *          that tile is empty, the result is equal
     *          to nullptr.
     */
    ChessmanPtr get_chessman_at(const Position &pos) const;

    /**
     * @return The side length of the chessboard.
     */
    unsigned get_side_length() const;

    /**
     * @param pos The position of the potentially beatable chessman.
     * @return True if the Chessman at the specified tile is owned
     *          by "the opposing player" and the currently selected
     *          chessman could move to _pos_.
     */
    bool is_beatable(const Position &pos) const;

    /**
     * @param p_chessman The chessman to move.
     * @param pos The move destination.
     * @return True if the chessman (who is assumed to be the currently
     *          selected one) can move to the specified position, beating
     *          the chessman at the destination if necessary.
     * @throws std::invalid_argument If the supplied chessman is NOT the
     *                               currently selected one.
     */
    bool is_move_valid(const ChessmanPtr &p_chessman, const Position &pos) const;

    /**
     * @param pos The source tile from which to move.
     * @param pos The move destination.
     * @return True if the chessman (who is assumed to be the currently
     *          selected one) can move to the specified position, beating
     *          the chessman at the destination if necessary.
     */
    bool is_move_valid(const Position &from, const Position &to) const;

    /**
     * @param pos The position of the chessman to pick up (/ "select").
     * @return True if there is a (friendly) chessman at the specified
     *         position and there is at least one possible move for
     *         this chessman.
     */
    bool is_pickable(const Position &pos) const;

    /**
     * @param pos The tile to check for availability.
     * @return Returns the most recently picked up chessman can
     *          be dropped at the specified position. (i. e. No
     *          other chessman obstruct the path and the specified
     *          tile is not occupied by a friendly chessman.)
     */
    bool is_current_chessman_droppable_at(const Position &pos) const;

    /**
     * Selects the chessman at the specified position.
     * If that chessman is not selectable, no action is taken.
     * @param pos The position of the chessman to select.
     */
    void pick(const Position &pos);

    /**
     * Attempts to drop the currently selected chessman at the
     * specified position. If that is not possible, no action is taken.
     * @param pos The tile where the currently selected chessman
     *            should be dropped.
     */
    void drop(const Position &pos);

    /**
     * Writes the chess board to the supplied output stream.
     * @param os The output stream to write to.
     */
    void render(std::ostream &os) const;

    /**
     * @return True if the king has been beaten.
     */
    bool essential_lost() const;

    /**
     * @return True if the player currently at action has not
     *          moved before.
     */
    bool is_first_move() const;

private: // methods

    /**
     * Writes the column decoration (letters) to the supplied
     * output stream.
     * @param os The output stream to write to.
     */
    void _render_col_chromium(std::ostream &os) const;

    /**
     * Writes a horizontal chess border to the supplied
     * output stream.
     * @param os The output stream to write to.
     */
    void _render_hor_border(std::ostream &os) const;

    /**
     * @param row The row of the tile.
     * @param pos The column of the tile.
     * @return The highlighting for the specified tile.
     */
    Highlight _get_highlight(Position pos) const;

    /**
     * Writes a part of the chessboard to the supplied output.
     * @param os The output stream to write to.
     * @param tile The character representation of the tile.
     * @param p_chessman The chessman to render. If p_chessman is a
     *                    nullptr, the character representation is rendered.
     * @param highlight The highlight for the given tile.
     */
    void _render_highlighted(std::ostream &os,
                             char tile,
                             const ChessmanPtr &p_chessman,
                             const Highlight &highlight) const;

private : // members

    /**
     * If this boi is set to true, the game can not be advanced any further.
     */
    bool _essential_lost {false};

    /**
     * The side length of the board.
     */
    unsigned _side_length;

    /**
     * The game this chessboard is associated with.
     */
    const Game &_parent_game;

    /**
     * The position of the currently selected chessman.
     * @Note If this is {0,'\0'}, no chessman is selected.
     */
    Position _current_selection {};

    /**
     * The matrix representing the chessboard.
     */
    std::vector<std::vector<ChessmanPtr>> _matrix{};

    /**
     * A map for bookkeeping which player has already moved a chessman.
     * This is only used for the pawns first move.
     */
    std::map<Color, bool> _first_move_map {
        {Color::WHITE, true},
        {Color::BLACK, true}
    };
};

} // namespace Chess