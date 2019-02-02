//       $Id: window.cpp 501 2018-12-06 18:36:01Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/src/gui/window.cpp $
// $Revision: 501 $
//     $Date: 2018-12-06 19:36:01 +0100 (Do., 06 Dez 2018) $
//   Creator: peter.kulczycki<AT>fh-hagenberg.at
//  Creation: June 1, 2018
//   $Author: p20068 $
// Copyright: (c) 2018 Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
//            (c) 2018 Michael Hava (michael.hava<AT>fh-hagenberg.at)
//   License: This document contains proprietary information belonging to
//            University of Applied Sciences Upper Austria, Campus Hagenberg. It
//            is distributed under the Boost Software License, Version 1.0 (see
//            http://www.boost.org/LICENSE_1_0.txt).

#include "../../inc/ml5/gui/window.h"

namespace ml5 {

void window::show_message_box (std::string const & text) {
   wxMessageBox (text);
}

window::window (std::string title) : m_prop_title {std::move (title)} {
}

window::~window () = default;

void window::add_menu (std::string const & title, mitem_cont_t const & items) const {
   get_frame ().add_menu (title, items);
}

void window::cursor_off () const {
   static wxCursor const blank {
      util::make_blank_cursor ()   // wxCURSOR_BLANK does not work with SetCursor
   };

   get_canvas ().SetCursor (blank);
}

void window::cursor_on () const {
   get_canvas ().SetCursor (wxCURSOR_ARROW);
}

application & window::get_app () const {
   return util::deref (m_p_app);
}

int window::get_height () const {
   return get_size ().y;
}

wxSize window::get_size () const {
   return get_canvas ().GetClientSize ();
}

int window::get_width () const {
   return get_size ().x;
}

void window::refresh () const {
   get_canvas ().Refresh ();
}

void window::restart_timer (std::chrono::milliseconds const interval) const {
   get_canvas ().restart_timer (interval);
}

void window::set_status_text (std::string const & text) const {
   get_frame ().SetStatusText (text);
}

void window::start_timer (std::chrono::milliseconds const interval) const {
   get_canvas ().start_timer (interval);
}

void window::stop_timer () const {
   get_canvas ().stop_timer ();
}

void window::on_exit () {
}

void window::on_init () {
}

void window::on_key (key_event const &) {
}

void window::on_menu (menu_event const &) {
}

void window::on_mouse_double_down (mouse_event const &) {
}

void window::on_mouse_down (mouse_event const &) {
}

void window::on_mouse_left_double_down (mouse_event const &) {
}

void window::on_mouse_left_down (mouse_event const &) {
}

void window::on_mouse_left_up (mouse_event const &) {
}

void window::on_mouse_move (mouse_event const &) {
}

void window::on_mouse_right_double_down (mouse_event const &) {
}

void window::on_mouse_right_down (mouse_event const &) {
}

void window::on_mouse_right_up (mouse_event const &) {
}

void window::on_mouse_up (mouse_event const &) {
}

void window::on_mouse_wheel (mouse_event const &) {
}

void window::on_paint (paint_event const &) {
}

void window::on_size (size_event const &) {
}

void window::on_timer (timer_event const &) {
}

impl::canvas & window::get_canvas () const {
   return get_frame ().get_canvas ();
}

impl::frame & window::get_frame () const {
   return util::deref (m_p_frame);
}

void window::set_app (application & app) {
   m_p_app = &app;
}

void window::set_frame (impl::frame * const p_frame) {
   m_p_frame = p_frame;
}

}   // namespace ml5
