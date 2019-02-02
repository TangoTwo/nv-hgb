//       $Id: impl_canvas.cpp 514 2018-12-07 15:47:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/src/gui/impl/impl_canvas.cpp $
// $Revision: 514 $
//     $Date: 2018-12-07 16:47:32 +0100 (Fr., 07 Dez 2018) $
//   Creator: peter.kulczycki<AT>fh-hagenberg.at
//  Creation: June 1, 2018
//   $Author: p20068 $
// Copyright: (c) 2018 Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
//            (c) 2018 Michael Hava (michael.hava<AT>fh-hagenberg.at)
//   License: This document contains proprietary information belonging to
//            University of Applied Sciences Upper Austria, Campus Hagenberg. It
//            is distributed under the Boost Software License, Version 1.0 (see
//            http://www.boost.org/LICENSE_1_0.txt).

#include "../../../inc/ml5/gui/impl/impl_canvas.h"

#include "../../../inc/ml5/gui/application.h"

namespace ml5::impl {

void canvas::restart_timer (std::chrono::milliseconds const interval) {
   stop_timer (); start_timer (interval);
}

void canvas::start_timer (std::chrono::milliseconds const interval) {
   using namespace std::chrono_literals;

   if (interval > 0ms) {
      m_timer.Start (static_cast <int> (interval.count ()));
   }
}

void canvas::stop_timer () {
   m_timer.Stop ();
}

canvas::canvas (window & win, wxWindow & parent)
#if wxVERSION_NUMBER >= 3100
   : wxGLCanvas {&parent, util::attributes (), 42}
#else
   : wxGLCanvas {&parent, 42, nullptr}
#endif
   , m_p_win    {&win} {

   initialize ();
}

ML5_DEFINE_GET_WIN_PROPERTY_DEF (canvas, background_brush, wxBrush)
ML5_DEFINE_GET_WIN_PROPERTY_DEF (canvas, paint_via_opengl, bool)

void canvas::bind_event_handlers () {
   if (win_prop_paint_via_opengl ()) {
      Bind (wxEVT_PAINT, [this] (wxPaintEvent const & event) {
         glViewport (0, 0, GetClientSize ().x, GetClientSize ().y);

         fire_paint (paint_event {event});

         glFlush     ();
         SwapBuffers ();
      });
   } else {
      Bind (wxEVT_PAINT, [this] (wxPaintEvent const & event) {
         wxAutoBufferedPaintDC context {this};

         context.SetBackground (win_prop_background_brush ());
         context.Clear ();

         fire_paint (paint_event {event, context});
      });
   }

   Bind (wxEVT_CHAR, [this] (wxKeyEvent const & event) {
      fire_key (key_event {event});
   });

   Bind (wxEVT_LEFT_DCLICK, [this] (wxMouseEvent const & event) {
      capture_mouse ();

      fire_mouse_ldd (mouse_event {event});
      fire_mouse_dd  (mouse_event {event});
   });

   Bind (wxEVT_LEFT_DOWN, [this] (wxMouseEvent const & event) {
      capture_mouse ();

      fire_mouse_ld   (mouse_event {event});
      fire_mouse_down (mouse_event {event});
   });

   Bind (wxEVT_LEFT_UP, [this] (wxMouseEvent const & event) {
      fire_mouse_lu (mouse_event {event});
      fire_mouse_up (mouse_event {event});

      release_mouse ();
   });

   Bind (wxEVT_MOTION, [this] (wxMouseEvent const & event) {
      fire_mouse_move (mouse_event {event});
   });

   Bind (wxEVT_MOUSEWHEEL, [this] (wxMouseEvent const & event) {
      fire_mouse_wheel (mouse_event {event});
   });

   Bind (wxEVT_RIGHT_DCLICK, [this] (wxMouseEvent const & event) {
      capture_mouse ();

      fire_mouse_rdd (mouse_event {event});
      fire_mouse_dd  (mouse_event {event});
   });

   Bind (wxEVT_RIGHT_DOWN, [this] (wxMouseEvent const & event) {
      capture_mouse ();

      fire_mouse_rd   (mouse_event {event});
      fire_mouse_down (mouse_event {event});
   });

   Bind (wxEVT_RIGHT_UP, [this] (wxMouseEvent const & event) {
      fire_mouse_ru (mouse_event {event});
      fire_mouse_up (mouse_event {event});

      release_mouse ();
   });

   Bind (wxEVT_SIZE, [this] (wxSizeEvent const & event) {
      fire_size (size_event {event});
   });

   Bind (wxEVT_TIMER, [this] (wxTimerEvent const & event) {
      fire_timer (timer_event {event});
   });
}

void canvas::capture_mouse () {
   if ((++m_captures == 1) && !HasCapture ()) {
      CaptureMouse ();
   }
}

void canvas::fire_key (key_event const & event) const {
   get_win ().on_key (event);
}

void canvas::fire_mouse_dd (mouse_event const & event) const {
   get_win ().on_mouse_double_down (event);
}

void canvas::fire_mouse_down (mouse_event const & event) const {
   get_win ().on_mouse_down (event);
}

void canvas::fire_mouse_ldd (mouse_event const & event) const {
   get_win ().on_mouse_left_double_down (event);
}

void canvas::fire_mouse_ld (mouse_event const & event) const {
   get_win ().on_mouse_left_down (event);
}

void canvas::fire_mouse_lu (mouse_event const & event) const {
   get_win ().on_mouse_left_up (event);
}

void canvas::fire_mouse_move (mouse_event const & event) const {
   get_win ().on_mouse_move (event);
}

void canvas::fire_mouse_rdd (mouse_event const & event) const {
   get_win ().on_mouse_right_double_down (event);
}

void canvas::fire_mouse_rd (mouse_event const & event) const {
   get_win ().on_mouse_right_down (event);
}

void canvas::fire_mouse_ru (mouse_event const & event) const {
   get_win ().on_mouse_right_up (event);
}

void canvas::fire_mouse_up (mouse_event const & event) const {
   get_win ().on_mouse_up (event);
}

void canvas::fire_mouse_wheel (mouse_event const & event) const {
   get_win ().on_mouse_wheel (event);
}

void canvas::fire_paint (paint_event const & event) const {
   get_win ().on_paint (event);
}

void canvas::fire_size (size_event const & event) const {
   get_win ().on_size (event);
}

void canvas::fire_timer (timer_event const & event) const {
   get_win ().on_timer (event);
   get_app ().on_timer (event);
}

ml5::application & canvas::get_app () const {
   return get_win ().get_app ();
}

window & canvas::get_win () const {
   return deref (m_p_win);
}

void canvas::initialize () {
   SetCurrent (util::deref (m_context = std::make_unique <wxGLContext> (this)));
}

void canvas::release_mouse () {
   if (((m_captures = std::max (0, m_captures - 1)) == 0) && HasCapture ()) {
      ReleaseMouse ();
   }
}

}   // namespace ml5::impl
