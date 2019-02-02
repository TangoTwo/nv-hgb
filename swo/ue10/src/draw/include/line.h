#pragma once

#include "shape.h"

class line : public shape
{
public:
    line() = default;

    explicit line(const wxPoint point) : shape { point }
    {}

    explicit line(const wxRect rect) : shape { rect }
    {}

    void draw_inverse(context_t &context) const override
    {
        wxPen thicc { m_pen.GetColour(), m_pen.GetWidth() + 2 };
        context.SetPen(thicc);
        on_draw(context);
    }

    std::unique_ptr <shape> clone() const override
    {
        return std::make_unique <line>(_rect);
    }

protected:
    void on_draw(context_t &context) const override
    {
        context.DrawLine(_rect.GetLeftTop(), _rect.GetBottomRight());
    }
};