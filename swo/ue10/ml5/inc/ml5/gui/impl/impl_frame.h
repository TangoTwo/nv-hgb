//       $Id: impl_frame.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/gui/impl/impl_frame.h $
// $Revision: 494 $
//     $Date: 2018-12-06 19:05:32 +0100 (Do., 06 Dez 2018) $
//   Creator: peter.kulczycki<AT>fh-hagenberg.at
//  Creation: June 1, 2018
//   $Author: p20068 $
// Copyright: (c) 2018 Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
//            (c) 2018 Michael Hava (michael.hava<AT>fh-hagenberg.at)
//   License: This document contains proprietary information belonging to
//            University of Applied Sciences Upper Austria, Campus Hagenberg. It
//            is distributed under the Boost Software License, Version 1.0 (see
//            http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include "./impl_canvas.h"

namespace ml5::impl {

class ML5_DLL_EXPORT frame final : public wxFrame {
   friend class application;

   public:
      using title_item_t = std::tuple <std::string, std::string>;
      using mitem_cont_t = std::vector <title_item_t>;

      frame (frame const &) = delete;
      frame (frame &&) = delete;

     ~frame () = default;

      frame & operator = (frame const &) = delete;
      frame & operator = (frame &&) = delete;

      void     add_menu   (std::string const & title, mitem_cont_t const & items);
      canvas & get_canvas () const;

   private:
      explicit frame (window &);

      ML5_DECLARE_GET_WIN_PROPERTY (allow_resize, bool)
      ML5_DECLARE_GET_WIN_PROPERTY (initial_size, wxSize)
      ML5_DECLARE_GET_WIN_PROPERTY (title,        std::string)

      void               bind_event_handlers ();
      void               broadcast_this      ();
      void               fire_exit           () const;
      void               fire_init           () const;
      void               fire_menu           (menu_event const &) const;
      ml5::application & get_app             () const;
      wxMenuBar &        get_menu_bar        () const;
      wxStatusBar &      get_status_bar      () const;
      window &           get_win             () const;
      void               initialize          ();
      wxMenu &           new_menu_about      ();
      wxMenuBar &        new_menu_bar        ();
      wxMenu &           new_menu_file       ();
      void               retreat_this        () const;

      bool                         m_init_fired    {false};     //
      int                          m_item_id       {101};       // 0 <= id < wxID_LOWEST
      std::map <int, title_item_t> m_menu_item_map {};          // item id, {menu title, item text}
      canvas *                     m_p_canvas      {nullptr};   //
      wxMenuBar *                  m_p_menubar     {nullptr};   //
      wxStatusBar *                m_p_statusbar   {nullptr};   //
      window *                     m_p_win         {nullptr};   //
};

}   // namespace ml5::impl
