#include "Board.hpp"
#include "Chessman.hpp"

namespace Chess {

std::istream &operator>>(std::istream &is, Position &pos)
{
    is >> pos.first >> pos.second;
    pos.first--; // chess starts with row 1, program logic with row 0
    return is;
}

std::ostream &operator<<(std::ostream &os, Color color)
{
    return os << '[' << (color == Color::WHITE ? "White" : "Black") << ']';
}

/**
 * @param c The character to "convert".
 * @return The 0-indexed column represented by the supplied character.
 *         This is mainly used for transformations between indices and
 *         chess board columns.
 */
static int char_to_col(char c)
{
    return std::tolower(c) - 'a';
}

Board::Board(const Game &game, unsigned side_length)
        : _side_length{side_length}, _parent_game{game}
{
    if (side_length < 8) {
        throw std::invalid_argument("Chess board side length must be greater than 8!");
    }

    // Initialize the chess board matrix to the correct dimensions
    _matrix.resize(get_side_length(), std::vector<ChessmanPtr>());
    for (auto &row : _matrix) {
        row.resize(get_side_length(), ChessmanPtr());
    }

    // write special black figures to first row
    _matrix[0][0] = std::make_shared<Rook>(Color::BLACK);
    _matrix[0][1] = std::make_shared<Knight>(Color::BLACK);
    _matrix[0][2] = std::make_shared<Bishop>(Color::BLACK);
    _matrix[0][3] = std::make_shared<King>(Color::BLACK);
    _matrix[0][4] = std::make_shared<Queen>(Color::BLACK);
    _matrix[0][5] = std::make_shared<Bishop>(Color::BLACK);
    _matrix[0][6] = std::make_shared<Knight>(Color::BLACK);
    _matrix[0][7] = std::make_shared<Rook>(Color::BLACK);

    // write black pawns to 2nd row
    for (auto &tile : _matrix[1]) {
        tile = std::make_shared<Pawn>(Color::BLACK);
    }

    // some complex mathematical computation involving calculus,
    // polar representation of the cartesian coordinate system
    // and one whole lot of cookies.
    const std::size_t last = get_side_length() - 1;
    const std::size_t snd_to_last = last - 1;

    // write white pawns to 2nd to last row
    for (auto &tile : _matrix[snd_to_last]) {
        tile = std::make_shared<Pawn>(Color::WHITE);
    }

    // write special white figures to last row
    _matrix[last][0] = std::make_shared<Rook>(Color::WHITE);
    _matrix[last][1] = std::make_shared<Knight>(Color::WHITE);
    _matrix[last][2] = std::make_shared<Bishop>(Color::WHITE);
    _matrix[last][3] = std::make_shared<King>(Color::WHITE);
    _matrix[last][4] = std::make_shared<Queen>(Color::WHITE);
    _matrix[last][5] = std::make_shared<Bishop>(Color::WHITE);
    _matrix[last][6] = std::make_shared<Knight>(Color::WHITE);
    _matrix[last][7] = std::make_shared<Rook>(Color::WHITE);
}

ChessmanPtr Board::get_chessman_at(const Position &pos) const
{
    auto row = pos.first;
    auto col = char_to_col(pos.second);
    ChessmanPtr p_chessman = nullptr;

    if (row < get_side_length() && col < get_side_length()) {
        p_chessman = _matrix[row][col];
    }
    return p_chessman;
}

unsigned Board::get_side_length() const
{
    return _side_length;
}

bool Board::is_beatable(const Position &pos) const
{
    bool beatable = false;
    auto p_chessman = get_chessman_at(pos);
    if (p_chessman != nullptr) {
        // the chessman is owned by the opponent
        beatable = p_chessman->color != _parent_game.get_current_player();
        // the chessman can be reached from _current_selection
        beatable = beatable && is_move_valid(_current_selection, pos);
    }
    return beatable;
}

bool Board::is_move_valid(const ChessmanPtr &p_chessman, const Position &pos) const
{
    if (get_chessman_at(_current_selection) != p_chessman) {
        // design flaw, but meh :/
        throw std::invalid_argument("The supplied chessman is not selected.");
    }
    // verifies that there is no friendly chessman @ pos
    auto target_cm = get_chessman_at(pos);
    bool noFriendly = target_cm == nullptr ||
                      (target_cm != nullptr && target_cm->color != _parent_game.get_current_player());

    // and the chessman supports the move
    return noFriendly && p_chessman != nullptr && p_chessman->can_move(*this, {_current_selection, pos});
}

bool Board::is_move_valid(const Position &from, const Position &to) const
{
    // this is the second M A J O R design flaw. IMO you should never need a const cast
    // except when using delegation for const overloads. The delegation here was necessary
    // due to improper planning.
    const auto &buff = _current_selection;
    const_cast<Board *>(this)->_current_selection = from;
    bool v = is_move_valid(get_chessman_at(from), to);
    const_cast<Board *>(this)->_current_selection = buff;
    return v;
}

static Position ints_to_pos(unsigned row, unsigned col)
{
    return { row, static_cast<char>('a' + col) };
}

bool Board::is_pickable(const Position &pos) const
{
    auto p_chessman = get_chessman_at(pos);
    bool pickable = false;
    if (p_chessman != nullptr) {
        // verifies that the chessman at _pos_ is owned by the current player
        bool correct_color = p_chessman->color == _parent_game.get_current_player();
        // and that the chessman can move at all.
        // if the color is not correct in the first place, there is no need
        // to check for possible moves. Hence the initialization with !correct_color.
        bool one_move_possible = !correct_color;
        int i = 0;
        unsigned tiles = get_side_length() * get_side_length();
        while (i < tiles && !one_move_possible) {
            Position tmp = ints_to_pos(i/get_side_length(), i%get_side_length());
            if (pos != tmp) {
                one_move_possible = is_move_valid(pos, tmp);
            }
            ++i;
        }
        pickable = correct_color && one_move_possible;
    }
    return pickable;
}

bool Board::is_current_chessman_droppable_at(const Position &pos) const
{
    return is_move_valid(_current_selection, pos);
}

void Board::pick(const Position &pos)
{
    // insane code, do not attempt
    // to comprehend its complexity
    // for the sake of your mental
    // health.
    if (is_pickable(pos)) {
        _current_selection = pos;
    }
}

void Board::drop(const Position &pos)
{
    if (get_chessman_at(_current_selection) != nullptr &&
        is_move_valid(_current_selection, pos)) {

        auto p_target = get_chessman_at(pos);
        if (p_target != nullptr && p_target->is_essential()) {
                _essential_lost = true;
        }
        // drop chessman (this also kicks previous chessman at _pos_ from the board)
        // also note that p_target is a smart pointer, kicked chessmen are deallocated autom.
        _matrix[pos.first][char_to_col(pos.second)] = get_chessman_at(_current_selection);
        _matrix[_current_selection.first][char_to_col(_current_selection.second)] = nullptr;
        _current_selection = {};

        _first_move_map[_parent_game.get_current_player()] = false;
    }
}

void Board::render(std::ostream &os) const
{
    static const char black = '=';
    static const char white = '+';
    char tile = black;

    _render_col_chromium(os);
    _render_hor_border(os);
    for (unsigned row = 0; row < _matrix.size(); ++row) {
        os << row + 1 << " |"; // left border
        for (unsigned col = 0; col < _matrix[row].size(); ++col) {
            const auto &p_chessman = _matrix[row][col];
            _render_highlighted(os, tile, p_chessman, _get_highlight(ints_to_pos(row, col)));
            tile = tile == black ? white : black; // checkerboard pattern!!!! :D
        }
        // switching tile sequence for each outer loop iteration
        // to achieve the alternation effect!
        tile = tile == black ? white : black;
        os << "| " << row + 1 << std::endl; // right border
    }
    _render_hor_border(os);
    _render_col_chromium(os);
}

void Board::_render_highlighted(std::ostream &os, char tile, const ChessmanPtr &p_chessman,
                                const Highlight &highlight) const
{
    os << highlight.first;
    if (p_chessman == nullptr) {
        os << tile;
    } else {
        p_chessman->render(os);
    }
    os << highlight.second;
}

Highlight Board::_get_highlight(Position pos) const
{
    Highlight highlight;
    auto p_chessman = get_chessman_at(pos);
    if (is_move_valid(get_chessman_at(_current_selection), pos)) {
        highlight = {'[', ']'};
    } else if (p_chessman != nullptr && p_chessman == get_chessman_at(_current_selection)) {
        highlight = {'(', ')'};
    } else {
        highlight = {' ', ' '};
    }
    return highlight;
}

bool Board::essential_lost() const
{
    return _essential_lost;
}

void Board::_render_col_chromium(std::ostream &os) const
{
    os << "  |";
    for (int i = 0; i < get_side_length(); ++i) {
        os << ' ' << static_cast<char>('a' + i) << ' ';
    }
    os << "|" << std::endl;
}

void Board::_render_hor_border(std::ostream &os) const
{
    os << "--+";
    for (int i = 0; i < get_side_length(); ++i) {
        os << "---";
    }
    os << "+--" << std::endl;
}

bool Board::is_first_move() const
{
    return _first_move_map.at(_parent_game.get_current_player());
}

}