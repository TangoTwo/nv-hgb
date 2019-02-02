//       $Id: integer.h 444 2018-07-31 19:00:26Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/ml5/Base/integer.h $
// $Revision: 444 $
//     $Date: 2018-07-31 21:00:26 +0200 (Di., 31 Jul 2018) $
//   Creator: michael.hava<AT>fh-hagenberg.at
//  Creation: June 1, 2018
//   $Author: p20068 $
// Copyright: (c) 2018 Michael Hava (michael.hava<AT>fh-hagenberg.at)
//            (c) 2018 Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
//   License: This document contains proprietary information belonging to
//            University of Applied Sciences Upper Austria, Campus Hagenberg. It
//            is distributed under the Boost Software License, Version 1.0 (see
//            http://www.boost.org/LICENSE_1_0.txt).

#pragma once

#include "./iterator.h"

namespace ml5 {

// -------------------------------------------------------------------------------------------------

template <typename T> container <T>::~container () = default;

template <typename T> bool container <T>::contains (T const & value) const noexcept {
   for (T const & entry : *this) {
      if (entry == value) {
         return true;
      }
   }

   return false;
}

template <typename T> std::size_t container <T>::size () const noexcept {
   std::size_t count {0};

   for (auto itor {make_iterator ()}; itor->not_at_end (); itor->to_next ()) {
      ++count;
   }

   return count;
}

template <typename T> util::iterator_adapter <T> container <T>::begin () const noexcept {
   return util::iterator_adapter <T> {make_iterator ()};
}

template <typename T> util::iterator_adapter <T> container <T>::end () const noexcept {
   return {};
}

template <typename T> bool container <T>::empty () const noexcept {
   return size () == 0;
}

template <typename T> iterator <T> * container <T>::new_iterator () const {
   return make_iterator ().release ();
}

// -------------------------------------------------------------------------------------------------

template <typename T> std::ostream & operator << (std::ostream & lhs, container <T> const & rhs) {
   lhs << '{'; bool first {true};

   for (T const & entry : rhs) {
      if (!first) {
         lhs << ',';
      }

      lhs << entry; first = false;
   }

   return lhs << '}';
}

// -------------------------------------------------------------------------------------------------

}   // namespace ml5
