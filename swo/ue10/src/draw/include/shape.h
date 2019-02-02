#pragma once

#include <ml5/ml5.h>

class shape : public ml5::object
{
public:
    using context_t = ml5::paint_event::context_t;

    shape() : _rect { 0, 0, 0, 0 }
    {};

    explicit shape(const wxPoint point) : _rect { point, point }
    {}

    explicit shape(const wxRect rect) : _rect { rect }
    {}

    /**
     * Prepares the graphical context for drawing the shape.
     */
    void draw(context_t &context) const
    {
        context.SetPen(m_pen);
        context.SetBrush(m_brush);

        on_draw(context);
    }

    /**
     * This method is only used for the icons within
     * the tool bar: Selected icons are drawn "inverse".
     */
    virtual void draw_inverse(context_t &context) const
    {
        context.SetPen(m_pen);
        context.SetBrush(*wxTRANSPARENT_BRUSH);
        on_draw(context);
    }

    /**
     * Adjusts width and height using the supplied corner
     * point relative to the origin (top left).
     */
    void set_right_bottom(const wxPoint point)
    {
        _rect.SetRightBottom(point);
    }

    /**
     * @return The rectangle surrounding the whole shape.
     */
    wxRect get_bounding_box() const
    {
        return _rect;
    }

    /**
     * Resizes the shape so that it touches the sides of
     * the supplied rectangle.
     */
    void set_bounding_box(const wxRect &rect)
    {
        _rect = rect;
    }

    /**
     * @return A deep copy of the shape.
     */
    virtual std::unique_ptr <shape> clone() const = 0;

protected:

    /**
     * A factory method that is called when drawing preparations
     * are finished.
     */
    virtual void on_draw(context_t &context) const = 0;

    /**
     * The default shape color.
     */
    wxBrush m_brush { wxBrush(wxColour(66, 134, 244)) };

    /**
     * The default border-color and -width of the shape.
     */
    wxPen m_pen { wxPen(wxColour(27, 77, 155), 2) };

    /**
     * The rectangle enclosing the shape.
     */
    wxRect _rect {};
};