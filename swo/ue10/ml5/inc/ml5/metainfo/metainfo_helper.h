//       $Id: metainfo_helper.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/metainfo/metainfo_helper.h $
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

#include "./metainfo_injector.h"

namespace ml5::util {

template <typename C> struct get_metainfo_helper final {
   template <bool A, typename ...B> static metainfo_injector <C, A, B...> deducer (metainfo_injector <C, A, B...> const &);

   static constexpr metainfo <C> const & get_meta_info () noexcept {
      return decltype (deducer (std::declval <C> ()))::get_metainfo ();
   }
};

}   // namespace ml5::util

namespace ml5 {

template <typename C> constexpr metainfo <C> const & get_metainfo () noexcept {
   return util::get_metainfo_helper <C>::get_meta_info ();
}

template <typename C, typename ...A> std::unique_ptr <C> instantiate (A && ...a) {
   return get_metainfo <C> ().instantiate (std::forward <A> (a)...);
}

}   // namespace ml5
