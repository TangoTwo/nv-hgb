#pragma once

#include "shape.h"

class rectangle final : public shape
{
public: // methods
    rectangle() = default;

    explicit rectangle(const wxPoint point) : shape { point }
    {}

    explicit rectangle(const wxRect rect) : shape { rect }
    {}

    std::unique_ptr <shape> clone() const override
    {
        return std::make_unique <rectangle>(_rect);
    }

private: // methods
    void on_draw(context_t &context) const override
    {
        context.DrawRectangle(_rect);
    }
};