#include "draw_window.h"

#include "ellipse.h"
#include "rectangle.h"
#include "line.h"
#include "arrow.h"

draw_window::draw_window() : ml5::window("ML.Draw"), _tool_bar { std::make_unique <tool_bar>(this, 70) }
{
    _make_shape = shape_registry.at("Ellipse");

    _tool_bar->add_item({
                                "Selection",
                                std::make_shared <arrow>(),
                                [this]() { this->_set_selection_mode(true); }
                        });
    _tool_bar->add_item({
                                "Ellipse",
                                std::make_shared <ellipse>(),
                                [this]() { this->_set_drawing_tool("Ellipse"); }
                        });
    _tool_bar->add_item({
                                "Line",
                                std::make_shared <line>(),
                                [this]() { this->_set_drawing_tool("Line"); }
                        });
    _tool_bar->add_item({
                                "Rectangle",
                                std::make_shared <rectangle>(),
                                [this]() { this->_set_drawing_tool("Rectangle"); }
                        });
}

void draw_window::on_init()
{
    window::on_init();
    add_menu("&Shape", {
            { "&Ellipse",   "draws an ellipse" },
            { "&Line",      "draws a line" },
            { "&Rectangle", "draws a rectangle" }
    });

    add_menu("&Edit", {
            { "&Copy",  "Copies the selected object" },
            { "C&ut",   "Moves the object from the canvas to the clipboard" },
            { "&Paste", "Copies the object from the clipboard to the canvas." }
    });

    // since the selection of a tool bar item emits an
    // event which is handled by a callback that causes a
    // context refresh, this call has to be down here and
    // cannot happen in the constructor.
    _tool_bar->select("Ellipse");
}

void draw_window::on_menu(ml5::menu_event const &event)
{
    if (event.get_title() == "Shape") {
        _tool_bar->select(event.get_item());
    } else if (event.get_title() == "Edit") {
        auto &item = event.get_item();
        if (item == "Copy" && _selection != -1) {
            copy_current();
        } else if (item == "Cut" && _selection != -1) {
            cut_current();
        } else if (item == "Paste") {
            paste_clipboard();
        }
    }
    window::on_menu(event);
}

void draw_window::on_paint(ml5::paint_event const &event)
{
    auto &context = event.get_context();

    // draw all shapes to the canvas
    for (std::size_t i = 0; i != _shapes.size(); ++i) {
        _shapes[i]->draw(context);
        if (i == _selection) {
            // selected shapes get a highlighter
            context.SetBrush(*wxTRANSPARENT_BRUSH);
            context.SetPen(wxPen(wxColour(100, 255, 100), 2, wxPenStyle::wxPENSTYLE_USER_DASH));
            context.DrawRectangle(_shapes[i]->get_bounding_box());
        }
    }

    // draw the "in progress" shape
    if (_shape) {
        _shape->draw(context);
    }

    // draw tool bar as overlay
    _tool_bar->draw(context);

    // draw keyboard shortcuts
    context.SetTextForeground(wxColour(0x222222));
    event.get_context().DrawText(
            "[Ctrl+C : Copy] [Ctrl+X : Cut] [Ctrl+V : Paste]",
            wxPoint(_tool_bar->get_bounding_box().width + 10, get_height() - 30));

    window::on_paint(event);
}

void draw_window::on_mouse_left_down(const ml5::mouse_event &event)
{
    if (_tool_bar->get_bounding_box().Contains(event.get_position())) {
        // delegate mouse event
        _tool_bar->on_mouse_left_down(event);
    } else if (_selecting) {
        // enable dragging mechanics
        _moving = true;

        // find selected shape
        int i = _shapes.size() - 1;
        while (i >= 0 && !_shapes[i]->get_bounding_box().Contains(event.get_position())) {
            --i;
        }
        _selection = i;
        refresh();
    } else {
        // create a new shape
        _shape = _make_shape(event.get_position());
    }
    window::on_mouse_left_down(event);
}

void draw_window::on_mouse_left_up(ml5::mouse_event const &event)
{
    if (_shape) {
        // add the final shape to the canvas if it is
        // big enough to be "relevant"
        auto box = _shape->get_bounding_box();
        if (box.width >= _MIN_OBJECT_SIZE || box.height >= _MIN_OBJECT_SIZE) {
            _shapes.add(std::move(_shape));
        } else {
            _shape.release();
        }
        refresh();
    } else if (_selecting) {
        _moving = false;
    }
    window::on_mouse_left_up(event);
}

void draw_window::on_mouse_move(ml5::mouse_event const &event)
{
    if (_shape) {
        // resize currently drawn shape
        _shape->set_right_bottom(event.get_position());
        refresh();
    } else if (_moving && _selection >= 0) {
        // move the selected shape
        auto box = _shapes[_selection]->get_bounding_box();
        box.SetTopLeft(event.get_position());
        _shapes[_selection]->set_bounding_box(box);
        refresh();
    }
    window::on_mouse_down(event);
}

void draw_window::_set_drawing_tool(const std::string &s)
{
    _make_shape = shape_registry.at(s);
    if (_selection != -1) {
        // morph selected shape into another
        // .. bit ugly but meh
        auto box = _shapes[_selection]->get_bounding_box();
        _shapes[_selection] = _make_shape(box.GetLeftTop());
        _shapes[_selection]->set_right_bottom(box.GetRightBottom());
    }
    _set_selection_mode(false);
    refresh();
}

void draw_window::on_key(const ml5::key_event &event)
{
    const auto &code = event.get_key_code();
    if (code == WXK_CONTROL_C) {
        copy_current();
    } else if (code == WXK_CONTROL_X) {
        cut_current();
    } else if (code == WXK_CONTROL_V) {
        paste_clipboard();
    }
    window::on_key(event);
}

void draw_window::copy_current()
{
    if (_selection != -1) {
        _clipboard = _shapes[_selection]->clone();
    }
}

void draw_window::cut_current()
{
    if (_selection != 1) {
        copy_current();
        auto &ptr = _shapes[_selection];
        _selection = -1;
        _shapes.remove(ptr);
        refresh();
    }
}

void draw_window::paste_clipboard()
{
    if (_clipboard != nullptr) {
        auto ptr = _clipboard->clone();
        auto box = ptr->get_bounding_box();
        box.x += 30;
        box.y += 30;
        ptr->set_bounding_box(box);
        _shapes.add(std::move(ptr));
        // select newly added shape
        _selection = _shapes.size() - 1;
        refresh();
    }
}

void draw_window::_set_selection_mode(bool flag)
{
    this->_selecting = flag;
    this->_selection = -1;
    refresh();
}
