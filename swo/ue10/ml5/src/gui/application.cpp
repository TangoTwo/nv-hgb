//       $Id: application.cpp 501 2018-12-06 18:36:01Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/src/gui/application.cpp $
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

#include "../../inc/ml5/gui/application.h"

namespace ml5 {

application::~application () = default;

window & application::get_win () const {
   return deref (m_p_win);
}

void application::on_exit () {
}

void application::on_init () {
}

void application::on_timer (timer_event const &) {
}

}   // namespace ml5
