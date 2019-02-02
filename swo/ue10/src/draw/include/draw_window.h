#pragma once

#include "shape.h"
#include "shape_registry.h"
#include "tool_bar.h"

class draw_window final : public ml5::window
{
public: // methods
    draw_window();

private: // methods
    static const unsigned _MIN_OBJECT_SIZE = 5;

    void on_init() override;

    void on_menu(ml5::menu_event const &event) override;

    void on_paint(ml5::paint_event const &event) override;

    void on_mouse_left_down(ml5::mouse_event const &event) override;

    void on_mouse_left_up(ml5::mouse_event const &) override;

    void on_mouse_move(ml5::mouse_event const &event) override;

protected: // methods
    void on_key(const ml5::key_event &event) override;

    /**
     * Copies the currently selected shape to the clipboard.
     */
    void copy_current();

    /**
     * Moves the currently selected shape from the canvas to the clipboard.
     */
    void cut_current();

    /**
     * Copies the clipboard contents to the canvas.
     */
    void paste_clipboard();

private: // methods
    /**
     * Prepares the canvas so that further interaction
     * is interpreted as the intention to draw the shape
     * as specified by the parameter.
     * @param s The name of the drawing tool.
     * @note This should be the key for the shape_registry (see shape_registry.h)
     */
    void _set_drawing_tool(const std::string &s);

    /**
     * Engages or disengages the shape selection mode
     * according to the supplied flag.
     * @param flag Whether the canvas should be set to
     *             selection mode.
     */
    void _set_selection_mode(bool flag);

private: // members
    /**
     * The side bar containing selectable drawing tools.
     */
    std::unique_ptr <tool_bar> _tool_bar;

    /**
     * The buffer which can hold copies of drawn shapes.
     */
    std::unique_ptr <shape> _clipboard;

    /**
     * The currently drawn shape.
     */
    std::unique_ptr <shape> _shape;

    /**
     * The list of shapes that are on the canvas.
     */
    ml5::vector <std::unique_ptr <shape>> _shapes;

    /**
     * The currently selected shape.
     * If no shape is selected, this
     * is -1.
     */
    int _selection { -1 }; // into _shapes

    /**
     * The function used for instantiating the correct
     * shape according to the current drawing mode.
     */
    shape_creator _make_shape;

    /**
     * True whenever the canvas is in selection mode.
     */
    bool _selecting;

    /**
     * True whenever the canvas is in selection mode and
     * a selected shape is moved.
     */
    bool _moving;
};
