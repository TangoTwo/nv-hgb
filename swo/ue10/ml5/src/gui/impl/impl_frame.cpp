//       $Id: impl_frame.cpp 503 2018-12-06 20:24:47Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/src/gui/impl/impl_frame.cpp $
// $Revision: 503 $
//     $Date: 2018-12-06 21:24:47 +0100 (Do., 06 Dez 2018) $
//   Creator: peter.kulczycki<AT>fh-hagenberg.at
//  Creation: June 1, 2018
//   $Author: p20068 $
// Copyright: (c) 2018 Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
//            (c) 2018 Michael Hava (michael.hava<AT>fh-hagenberg.at)
//   License: This document contains proprietary information belonging to
//            University of Applied Sciences Upper Austria, Campus Hagenberg. It
//            is distributed under the Boost Software License, Version 1.0 (see
//            http://www.boost.org/LICENSE_1_0.txt).

#include "../../../inc/ml5/gui/impl/impl_frame.h"

#include "../../../inc/ml5/gui/application.h"

namespace ml5::impl {

void frame::add_menu (std::string const & title, mitem_cont_t const & items) {
   wxMenu & menu {util::deref_new <wxMenu> ()};

   for (auto const & [t, h] : items) {
      menu.Append (m_item_id, t, h);

      Bind (wxEVT_MENU, [this] (wxCommandEvent const & event) {
         fire_menu (menu_event {event, m_menu_item_map[event.GetId ()]});
      }, m_item_id);

      m_menu_item_map[m_item_id++] = {util::remove_all (title, '&'), util::remove_all (t, '&')};
   }

   get_menu_bar ().Insert (
      get_menu_bar ().GetMenuCount () - 1, &menu, title   // right before the 'About' menu
   );
}

canvas & frame::get_canvas () const {
   return util::deref (m_p_canvas);
}

frame::frame (window & win) : wxFrame {nullptr, 4711, ""}, m_p_win {&win} {
   initialize ();
}

ML5_DEFINE_GET_WIN_PROPERTY_DEF (frame, allow_resize, bool)
ML5_DEFINE_GET_WIN_PROPERTY_DEF (frame, initial_size, wxSize)
ML5_DEFINE_GET_WIN_PROPERTY_DEF (frame, title,        std::string)

void frame::bind_event_handlers () {
   Bind (wxEVT_CLOSE_WINDOW, [this] (wxCloseEvent const &) {
      get_canvas ().stop_timer ();

      fire_exit    ();
      retreat_this ();

      Destroy ();
   });

   Bind (wxEVT_SIZE, [this] (wxSizeEvent const &) {   // is this tight binding necessary?
      get_canvas ().SetSize (GetClientSize ());

      if (!m_init_fired) {
         fire_init (); m_init_fired = true;   // in order to have a valid size in on_init
      }
   });

   get_canvas ().bind_event_handlers ();
}

void frame::broadcast_this () {
   get_win ().set_frame (this);
}

void frame::fire_exit () const {
   get_win ().on_exit ();
   get_app ().on_exit ();
}

void frame::fire_init () const {
   get_app ().on_init ();
   get_win ().on_init ();
}

void frame::fire_menu (menu_event const & event) const {
   get_win ().on_menu (event);
}

ml5::application & frame::get_app () const {
   return get_win ().get_app ();
}

wxMenuBar & frame::get_menu_bar () const {
   return util::deref (m_p_menubar);
}

wxStatusBar & frame::get_status_bar () const {
   return util::deref (m_p_statusbar);
}

window & frame::get_win () const {
   return deref (m_p_win);
}

void frame::initialize () {
   if (!win_prop_allow_resize ()) {
      SetWindowStyle (GetWindowStyle () & ~wxMAXIMIZE_BOX & ~wxRESIZE_BORDER);
   }

   SetTitle      (win_prop_title ());
   SetClientSize (win_prop_initial_size ());

   m_p_menubar   = &new_menu_bar ();
   m_p_statusbar = CreateStatusBar ();

   SetMenuBar (m_p_menubar);

   auto const & [w, h] {GetClientSize ()};

   SetClientSize (
      w, h + get_status_bar ().GetSize ().y   // + get_menu_bar ().GetSize ().y   // !pwk: not valid here
   );

   m_p_canvas = new canvas {get_win (), *this};

   broadcast_this ();
   bind_event_handlers ();
}

wxMenu & frame::new_menu_about () {
   wxMenu & menu {util::deref_new <wxMenu> ()}; menu.Append (wxID_ABOUT);

   Bind (wxEVT_MENU, [] (wxCommandEvent const &) {
      wxMessageBox ("blah ...", "About MiniLib 5", wxOK | wxICON_INFORMATION);
   }, wxID_ABOUT);

   return menu;
}

wxMenuBar & frame::new_menu_bar () {
   wxMenuBar & menubar {util::deref_new <wxMenuBar> ()};

   menubar.Append (&new_menu_file  (), "&File");
   menubar.Append (&new_menu_about (), "&Help");

   return menubar;
}

wxMenu & frame::new_menu_file () {
   wxMenu & menu {util::deref_new <wxMenu> ()}; menu.Append (wxID_EXIT);

   Bind (wxEVT_MENU, [this] (wxCommandEvent const &) {
      Close ();
   }, wxID_EXIT);

   return menu;
}

void frame::retreat_this () const {
   get_win ().set_frame (nullptr);
}

}   // namespace ml5::impl
