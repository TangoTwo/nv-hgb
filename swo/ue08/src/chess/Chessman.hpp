#pragma once

#include <map>
#include "Chess.hpp"

class Chessman
{

public: // methods

    /**
     * A figure can be instanciated with a color
     * @param color_ The color of the chessman.
     */
    explicit Chessman(Color color_)
    : color {color_} { }

    /**
     * @return The "visual" representation of the chessman.
     */
    virtual char get_representation() const = 0;

    /**
     * @return True if the removal of a concrete chessman ends a game.
     */
    virtual bool is_essential() const;

    /**
     * @param move The move to verify
     * @return True if the supplied move is valid vor a specific chessman.
     */
    virtual bool can_move(const std::pair<Chess::Position, Chess::Position> & move) const = 0;

public: // members

    /**
     * Guess what aspect of an objects state this lil' member describes :s
     */
    const Color color;
};

class King : public Chessman
{
public: // methods
    explicit King(Color color_)
    : Chessman {color_} { }

    char get_representation() const override;
    bool can_move(const std::pair<Chess::Position, Chess::Position> &move) const override;
    bool is_essential() const override;
};

class Queen : public Chessman
{
public: // methods
    explicit Queen(Color color_)
            : Chessman {color_} { }

    char get_representation() const override;
    bool can_move(const std::pair<Chess::Position, Chess::Position> &move) const override;
};

class Bishop : public Chessman
{
public: // methods
    explicit Bishop(Color color_)
            : Chessman {color_} { }

    char get_representation() const override;
    bool can_move(const std::pair<Chess::Position, Chess::Position> &move) const override;
};

class Rook : public Chessman
{
public: // methods
    explicit Rook(Color color_)
            : Chessman {color_} { }

    char get_representation() const override;
    bool can_move(const std::pair<Chess::Position, Chess::Position> &move) const override;
};

class Knight : public Chessman
{
public: // methods
    explicit Knight(Color color_)
            : Chessman {color_} { }

    char get_representation() const override;
    bool can_move(const std::pair<Chess::Position, Chess::Position> &move) const override;
};

class Pawn : public Chessman
{
public: // methods
    explicit Pawn(Color color_)
            : Chessman {color_} { }

    char get_representation() const override;
    bool can_move(const std::pair<Chess::Position, Chess::Position> &move) const override;
};