//       $Id: impl_application.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/gui/impl/impl_application.h $
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

#include "./wx.h"

namespace ml5::impl {

class ML5_DLL_EXPORT application final : public wxApp {
   friend class ml5::application;

   public:
      application (application const &) = delete;
      application (application &&) = delete;

     ~application () = default;

      application & operator = (application const &) = delete;
      application & operator = (application &&) = delete;

      bool OnInit () override;

      ml5::application & get_app () const;
      window &           get_win () const;

   private:
      explicit application (ml5::application &);

      wxFrame & new_frame () const;

      ml5::application * m_p_app {nullptr};
};

}   // namespace ml5::impl
