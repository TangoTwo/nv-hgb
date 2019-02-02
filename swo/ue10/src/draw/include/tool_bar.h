#pragma once

#include <wx/dc.h>
#include <functional>
#include <memory>
#include "shape.h"

class draw_window;

/**
 * A simple callback for event handling.
 */
using event_handler = std::function <void()>;

/**
 * A clickable item displayed within a tool bar.
 */
struct tool_bar_item
{
    /**
     * The name of the goofball.
     */
    std::string name;

    /**
     * The visual representation of the item.
     */
    std::shared_ptr <shape> icon;

    /**
     * The function which is invoked upon clicking
     * the icon of the item.
     */
    event_handler callback;
};

class tool_bar
{
public: // methods
    /**
     * @param parent The window in which this tool bar will be mounted.
     * @param width The desired with of the tool bar.
     */
    explicit tool_bar(draw_window *parent, unsigned width);

    /**
     * Draws the toolbar
     */
    void draw(wxDC &context) const;

    /**
     * Adds the supplied item to the toolbar. The later an item is
     * added, the further down in the bar its icon will appear.
     * @param item The item to add.
     */
    void add_item(tool_bar_item item);

    /**
     * @return The metrics of the tool bar.
     */
    wxRect get_bounding_box() const;

    /**
     * Processes clicks that happen within the tool bar.
     */
    void on_mouse_left_down(const ml5::mouse_event &event);

    /**
     * Selects the tool bar item with the specified name.
     */
    void select(const std::string &name);

private: // members

    /**
     * The spacing between the tool bar border and
     * the icons of its items.
     */
    static const unsigned PADDING = 10;

    /**
     * @see tool_bar::tool_bar(draw_window *parent, unsigned width);
     */
    draw_window *const _parent;

    /**
     * @see tool_bar::tool_bar(draw_window *parent, unsigned width);
     */
    unsigned _width;

    /**
     * The list of items held by the tool bar.
     */
    std::vector <tool_bar_item> _items;

    /**
     * The index of the selected tool in tool_bar::_items.
     */
    std::size_t _selection { 0 };

    /**
     * The background color of the tool bar.
     */
    wxBrush _brush { wxBrush(wxColour(230, 230, 230)) };

    /**
     * The border-color and -width of the tool bar.
     */
    wxPen _pen { *wxBLACK_PEN };
};