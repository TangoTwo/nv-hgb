//       $Id: metainfo.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/metainfo/metainfo.h $
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

#include "./metainfo_registry.h"

#if !defined ML5_TRANSLATION_UNIT
   #define ML5_TRANSLATION_UNIT unknown
#endif

// -------------------------------------------------------------------------------------------------

namespace ml5 {

template <typename C> struct metainfo final : metainfo_base {
   explicit constexpr metainfo (bool abstract, minfo_cont_t bases = {});

  ~metainfo ();

   template <typename ...A> std::unique_ptr <C> instantiate (A && ...a) const;
};

template <typename C> constexpr metainfo <C> const & get_metainfo () noexcept;
template <typename C, typename ...A> std::unique_ptr <C> instantiate  (A && ...a);

}   // namespace ml5

// -------------------------------------------------------------------------------------------------

#include "./metainfo.inl"
