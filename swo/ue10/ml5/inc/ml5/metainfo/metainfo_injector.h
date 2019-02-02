//       $Id: metainfo_injector.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/metainfo/metainfo_injector.h $
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

#include "./metainfo.h"

namespace ml5::util {

template <typename C, bool A, typename ...B> struct metainfo_injector {
   metainfo_injector () noexcept {
      get_metainfo ().count_construction ();
   }

   metainfo_injector (metainfo_injector const &) noexcept {
      get_metainfo ().count_construction ();
   }

   metainfo_injector (metainfo_injector &&) noexcept {
      get_metainfo ().count_construction ();
   }

  ~metainfo_injector () noexcept {
      get_metainfo ().count_destruction ();
   }

   metainfo_injector & operator = (metainfo_injector const &) = default;
   metainfo_injector & operator = (metainfo_injector &&) = default;

   static metainfo <C> & get_metainfo () noexcept {
      static metainfo <C> minfo {A, {ml5::get_metainfo <B> ()...}}; return minfo;   // !pwk
   }
};

}   // namespace ml5::util
