#include "tool_bar.h"

#include <iostream>
#include "draw_window.h"

tool_bar::tool_bar(draw_window *parent, unsigned width) : _parent { parent }, _width { width }
{}

void tool_bar::draw(wxDC &context) const
{
    // draw the tool bar
    context.SetPen(_pen);
    context.SetBrush(_brush);
    context.DrawRectangle(0, 0, _width, _parent->get_height());

    // draw them icons
    for (std::size_t i = 0; i < _items.size(); ++i) {
        if (i != _selection) {
            _items[i].icon->draw(context);
        } else {
            _items[i].icon->draw_inverse(context);
        }
    }

}

void tool_bar::add_item(tool_bar_item item)
{
    // scale and position the icon relative to the
    // other items in the tool bar
    wxRect relative { PADDING, PADDING, static_cast<int>(_width - PADDING * 2),
                      static_cast<int>(_width - PADDING * 2) };
    relative.y = static_cast<int>(PADDING +
                                  (_items.size() * PADDING * 2) /* padding per item */ +
                                  (_items.size() * (_width - PADDING * 2))); /* items */
    item.icon->set_bounding_box(relative);

    _items.push_back(item);
}

wxRect tool_bar::get_bounding_box() const
{
    return wxRect(0, 0, _width, _parent->get_height());
}

void tool_bar::on_mouse_left_down(const ml5::mouse_event &event)
{
    std::size_t i = 0;
    while (i != _items.size() && !_items[i].icon->get_bounding_box().Contains(event.get_position())) {
        ++i;
    }
    if (i != _items.size()) {
        // an icon was clicked!
        _selection = i;
        _items[i].callback();
    }
}

void tool_bar::select(const std::string &s)
{
    std::size_t i = 0;
    while (i != _items.size() && s != _items[i].name) {
        ++i;
    }
    if (i != _items.size()) {
        _selection = i;
        _items[i].callback();
    }
}
