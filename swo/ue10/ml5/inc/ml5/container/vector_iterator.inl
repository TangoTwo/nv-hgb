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

namespace ml5 {

template <typename T> T & vector_iterator <T>::get_current () const {
   if (is_at_end ())
      throw std::out_of_range {"ml5::vector_iterator<T>::get_current: cannot dereference end iterator"};

   return *m_ptr;
}

template <typename T> bool vector_iterator <T>::is_at_end () const noexcept {
   return m_ptr == m_end;
}

template <typename T> void vector_iterator <T>::to_next () {
   if (is_at_end ())
      throw std::out_of_range {"ml5::vector_iterator<T>::to_next: cannot increment end iterator"};

   ++m_ptr;
}

template <typename T> vector_iterator <T>::vector_iterator (T * const ptr, std::size_t const size)
   : m_ptr {ptr}
   , m_end {ptr + size} {
}

}   // namespace ml5
