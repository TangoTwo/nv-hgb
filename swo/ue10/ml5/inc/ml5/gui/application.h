//       $Id: application.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/gui/application.h $
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

#include "./impl/impl_application.h"

#include "./window.h"

// -------------------------------------------------------------------------------------------------

namespace ml5 {

class ML5_DLL_EXPORT application : public object, MI5_DERIVE (application, object) {
   MI5_INJECT (application)

   friend class impl::canvas;
   friend class impl::frame;

   public:
      application () = default;

      application (application const &) = delete;
      application (application &&) = delete;

      virtual ~application () = 0;

      application & operator = (application const &) = delete;
      application & operator = (application &&) = delete;

                               window & get_win () const;
      template <typename ...A> int      run     (A && ...args);

   protected:
      virtual std::unique_ptr <window> make_window () const = 0;

      virtual void on_exit  ();
      virtual void on_init  ();
      virtual void on_timer (timer_event const & event);

   private:
      impl::application *      m_p_app_impl {nullptr};
      std::unique_ptr <window> m_p_win      {nullptr};
};

}   // namespace ml5

// -------------------------------------------------------------------------------------------------

#include "./application.inl"
