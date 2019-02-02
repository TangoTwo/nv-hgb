//       $Id: events.cpp 501 2018-12-06 18:36:01Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/src/gui/events.cpp $
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

#include "../../inc/ml5/gui/events.h"

namespace ml5 {

// -------------------------------------------------------------------------------------------------

event::~event () = default;

// -------------------------------------------------------------------------------------------------

key_event::key_event (wxKeyEvent const & e) : event {e} {
}

int key_event::get_key_code () const {
   return get_event ().GetKeyCode ();
}

wxKeyEvent const & key_event::get_event () const {
   return event::get_event <wxKeyEvent> ();
}

// -------------------------------------------------------------------------------------------------

menu_event::menu_event (wxCommandEvent const & e, std::tuple <std::string, std::string> const & item)
   : event  {e}
   , m_item {item} {
}

std::string const & menu_event::get_item () const {
   return std::get <1> (m_item);
}

std::string const & menu_event::get_title () const {
   return std::get <0> (m_item);
}

std::string menu_event::get_title_and_item () const {
   return get_title () + "/" + get_item ();
}

wxCommandEvent const & menu_event::get_event () const {
   return event::get_event <wxCommandEvent> ();
}

// -------------------------------------------------------------------------------------------------

mouse_event::mouse_event (wxMouseEvent const & e) : event {e} {
}

int mouse_event::get_button () const {
   return get_event ().GetButton ();
}

wxPoint mouse_event::get_position () const {
   return get_event ().GetPosition ();
}

int mouse_event::get_wheel_rotation () const {
   return get_event ().GetWheelRotation ();
}

bool mouse_event::is_left_button () const {
   return get_button () == wxMOUSE_BTN_LEFT;
}

bool mouse_event::is_right_button () const {
   return get_button () == wxMOUSE_BTN_RIGHT;
}

wxMouseEvent const & mouse_event::get_event () const {
   return event::get_event <wxMouseEvent> ();
}

// -------------------------------------------------------------------------------------------------

paint_event::paint_event (wxPaintEvent const & e) : event {e} {
}

paint_event::paint_event (wxPaintEvent const & e, context_t & context)
   : event     {e}
   , m_context {context} {
}

paint_event::context_t & paint_event::get_context () const {
   return util::unwrap (m_context);
}

bool paint_event::has_context () const {
   return m_context.has_value ();
}

wxPaintEvent const & paint_event::get_event () const {
   return event::get_event <wxPaintEvent> ();
}

// -------------------------------------------------------------------------------------------------

size_event::size_event (wxSizeEvent const & e) : event {e} {
}

wxSize size_event::get_size () const {
   return get_event ().GetSize ();
}

wxSizeEvent const & size_event::get_event () const {
   return event::get_event <wxSizeEvent> ();
}

// -------------------------------------------------------------------------------------------------

timer_event::timer_event (wxTimerEvent const & e) : event {e} {
}

wxTimerEvent const & timer_event::get_event () const {
   return event::get_event <wxTimerEvent> ();
}

// -------------------------------------------------------------------------------------------------

}   // namespace ml5
