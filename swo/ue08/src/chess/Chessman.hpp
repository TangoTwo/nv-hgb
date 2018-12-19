#pragma once

#include <map>
#include <memory>
#include "Types.hpp"
#include "Game.hpp"

namespace Chess {

class Chessman
{
public: // methods

    /**
     * A figure can only be instanciated with a color
     * @param color_ The color of the chessman.
     */
    explicit Chessman(Color color_)
            : color{color_}
    {}

    /**
     *  Writes a chessmans representation to the
     *  supplied output stream.
     */
    void render(std::ostream &os) const;

    /**
     * @return True if the removal of a concrete chessman ends a game of chess.
     */
    virtual bool is_essential() const;

    /**
     * @param move The move to verify.
     * @return True if the supplied move is valid vor a specific chessman.
     */
    virtual bool can_move(const Chess::Board &board, const Move &move) const = 0;

public: // members

    /**
     * Describes the circumference of the chessmans body
     * multiplied by the maximum number of tiles the chessman
     * can move within one turn.
     */
    const Color color;

protected: // methods
    /**
     * @return The "visual" representation of the chessman.
     */
    virtual char get_representation() const = 0;
};

class King : public Chessman
{
public: // methods
    explicit King(Color color_)
            : Chessman{color_}
    {}

    bool can_move(const Chess::Board &board, const Move &move) const override;

    bool is_essential() const override;

protected: // methods
    char get_representation() const override;
};

class Queen : public Chessman
{
public: // methods
    explicit Queen(Color color_)
            : Chessman{color_}
    {}

    bool can_move(const Chess::Board &board, const Move &move) const override;

protected: // methods
    char get_representation() const override;
};

class Bishop : public Chessman
{
public: // methods
    explicit Bishop(Color color_)
            : Chessman{color_}
    {}

    bool can_move(const Chess::Board &board, const Move &move) const override;

protected: // methods
    char get_representation() const override;
};

class Rook : public Chessman
{
public: // methods
    explicit Rook(Color color_)
            : Chessman{color_}
    {}

    bool can_move(const Chess::Board &board, const Move &move) const override;

protected: // methods
    char get_representation() const override;
};

class Knight : public Chessman
{
public: // methods
    explicit Knight(Color color_)
            : Chessman{color_}
    {}

    bool can_move(const Chess::Board &board, const Move &move) const override;

protected: // methods
    char get_representation() const override;
};

class Pawn : public Chessman
{
public: // methods
    explicit Pawn(Color color_)
            : Chessman{color_}
    {}

    bool can_move(const Chess::Board &board, const Move &move) const override;

protected: // methods
    char get_representation() const override;
};

} // namespace Chess