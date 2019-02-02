//       $Id: window.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/gui/window.h $
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

#include "./impl/impl_frame.h"

namespace ml5 {

class ML5_DLL_EXPORT window : public object, MI5_DERIVE (window, object) {
   MI5_INJECT (window)

   friend class application;
   friend class impl::canvas;
   friend class impl::frame;

   public:
      using title_item_t = impl::frame::title_item_t;
      using mitem_cont_t = impl::frame::mitem_cont_t;

      static void show_message_box (std::string const & text);

      window () = default;

      window (window const &) = delete;
      window (window &&) = delete;

      explicit window (std::string title);

      virtual ~window () = 0;

      window & operator = (window const &) = delete;
      window & operator = (window &&) = delete;

      void          add_menu        (std::string const & title, mitem_cont_t const & items) const;
      void          cursor_off      () const;
      void          cursor_on       () const;
      application & get_app         () const;
      int           get_height      () const;
      wxSize        get_size        () const;
      int           get_width       () const;
      void          refresh         () const;
      void          restart_timer   (std::chrono::milliseconds interval) const;
      void          set_status_text (std::string const & text) const;
      void          start_timer     (std::chrono::milliseconds interval) const;
      void          stop_timer      () const;

   protected:
      virtual void on_exit                    ();
      virtual void on_init                    ();
      virtual void on_key                     (key_event const & event);
      virtual void on_menu                    (menu_event const & event);
      virtual void on_mouse_double_down       (mouse_event const & event);
      virtual void on_mouse_down              (mouse_event const & event);
      virtual void on_mouse_left_double_down  (mouse_event const & event);
      virtual void on_mouse_left_down         (mouse_event const & event);
      virtual void on_mouse_left_up           (mouse_event const & event);
      virtual void on_mouse_move              (mouse_event const & event);
      virtual void on_mouse_right_double_down (mouse_event const & event);
      virtual void on_mouse_right_down        (mouse_event const & event);
      virtual void on_mouse_right_up          (mouse_event const & event);
      virtual void on_mouse_up                (mouse_event const & event);
      virtual void on_mouse_wheel             (mouse_event const & event);
      virtual void on_paint                   (paint_event const & event);
      virtual void on_size                    (size_event const & event);
      virtual void on_timer                   (timer_event const & event);

      impl::canvas & get_canvas () const;
      impl::frame &  get_frame  () const;

   private:
      void set_app   (application & app);
      void set_frame (impl::frame * p_frame);

      ML5_DEFINE_WIN_PROPERTY (allow_resize,     bool,        true)
      ML5_DEFINE_WIN_PROPERTY (background_brush, wxBrush,    *wxWHITE_BRUSH)
      ML5_DEFINE_WIN_PROPERTY (initial_size,     wxSize,      wxDefaultSize)
      ML5_DEFINE_WIN_PROPERTY (paint_via_opengl, bool,        false)
      ML5_DEFINE_WIN_PROPERTY (title,            std::string, "MiniLib 5")

      application * m_p_app   {nullptr};
      impl::frame * m_p_frame {nullptr};
};

}   // namespace ml5
