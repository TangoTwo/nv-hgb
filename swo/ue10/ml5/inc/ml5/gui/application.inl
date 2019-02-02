//       $Id: integer.h 444 2018-07-31 19:00:26Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/ml5/Base/integer.h $
// $Revision: 444 $
//     $Date: 2018-07-31 21:00:26 +0200 (Di., 31 Jul 2018) $
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

namespace ml5 {

template <typename ...A> int application::run (A && ...args) {
   m_p_win      = make_window ();
   m_p_app_impl = new impl::application {*this};

   get_win ().set_app (*this);

   return wxEntry (std::forward <A> (args)...);
}

}   // namespace ml5
