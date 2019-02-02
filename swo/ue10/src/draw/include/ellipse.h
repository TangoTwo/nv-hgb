#pragma once

#include "shape.h"

class ellipse final : public shape
{
public: // methods
    ellipse() = default;

    explicit ellipse(const wxPoint point) : shape { point }
    {}

    explicit ellipse(const wxRect rect) : shape { rect }
    {}

    std::unique_ptr <shape> clone() const override
    {
        return std::make_unique <ellipse>(_rect);
    }

private: // methods
    void on_draw(context_t &context) const override
    {
        context.DrawEllipse(_rect);
    }
};