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

template <typename C> constexpr metainfo <C>::metainfo (bool const abstract, minfo_cont_t bases)
   : metainfo_base {
        std::is_abstract_v <C> || abstract,
        util::type_info <C> (),
        std::move (bases)
     } {
   MI5_REGISTRY.enregister (ML5_STRINGIZE_MACRO (ML5_TRANSLATION_UNIT), *this);
}

template <typename C> metainfo <C>::~metainfo () {
   MI5_REGISTRY.deregister (ML5_STRINGIZE_MACRO (ML5_TRANSLATION_UNIT), *this);
}

template <typename C> template <typename ...A> std::unique_ptr <C> metainfo <C>::instantiate (A && ...a) const {
   if (m_abstract) {
      throw std::runtime_error {
         "ml5::metainfo<C>::instantiate<...A>: cannot instantiate abstract class '" + util::deref (m_p_name) + "'"
      };
   }

   return std::make_unique <C> (std::forward <A> (a)...);
}

}   // namespace ml5
