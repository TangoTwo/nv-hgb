#pragma once

#include "shape.h"

class arrow : public shape
{
public: // methods
    arrow() = default;

    std::unique_ptr <shape> clone() const override
    {
        return std::unique_ptr <arrow>();
    }

    void draw_inverse(context_t &context) const override
    {
        wxPen thicc { m_pen.GetColour(), m_pen.GetWidth() + 2 };
        context.SetPen(thicc);
        on_draw(context);
    }

protected: // methods
    void on_draw(context_t &context) const override
    {
        auto root = _rect.GetTopLeft();

        // "shaft"
        context.DrawLine(root, _rect.GetBottomRight());

        // "legs"
        auto to_right = root;
        to_right.x += (_rect.GetWidth() / 3);
        context.DrawLine(root, to_right);

        auto to_bottom = root;
        to_bottom.y += (_rect.GetWidth() / 3); // Using _width_ by intention!
        context.DrawLine(root, to_bottom);
    }
};