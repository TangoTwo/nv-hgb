//       $Id: object.cpp 501 2018-12-06 18:36:01Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/src/base/object.cpp $
// $Revision: 501 $
//     $Date: 2018-12-06 19:36:01 +0100 (Do., 06 Dez 2018) $
//   Creator: michael.hava<AT>fh-hagenberg.at
//  Creation: June 1, 2018
//   $Author: p20068 $
// Copyright: (c) 2018 Michael Hava (michael.hava<AT>fh-hagenberg.at)
//            (c) 2018 Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
//   License: This document contains proprietary information belonging to
//            University of Applied Sciences Upper Austria, Campus Hagenberg. It
//            is distributed under the Boost Software License, Version 1.0 (see
//            http://www.boost.org/LICENSE_1_0.txt).

#include "../../inc/ml5/base/object.h"

namespace ml5 {

object::~object () = default;

std::string const & object::get_class_name () const noexcept {
   return get_metainfo ().name ();
}

object::minfo_cont_t const & object::get_metainfo_bases () const noexcept {
   return get_metainfo ().bases ();
}

bool object::is_a (std::string const & name) const noexcept {
   return get_metainfo ().is_a (name);
}

bool object::is_a (std::type_info const & type) const noexcept {
   return get_metainfo ().is_a (type);
}

}   // namespace ml5
