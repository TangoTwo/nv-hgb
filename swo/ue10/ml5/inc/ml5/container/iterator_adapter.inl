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

namespace ml5::util {

template <typename T> iterator_adapter <T>::iterator_adapter (std::unique_ptr <iterator <T>> itor)
   : m_itor {std::move (itor)} {

   release_if_at_end ();
}

template <typename T> bool iterator_adapter <T>::operator == (iterator_adapter const & rhs) noexcept {
   return m_itor == rhs.m_itor;
}

template <typename T> bool iterator_adapter <T>::operator != (iterator_adapter const & rhs) noexcept {
   return !(*this == rhs);
}

template <typename T> iterator_adapter <T> & iterator_adapter <T>::operator ++ () {
   if (is_end_iterator ()) {
      throw std::out_of_range {
         "ml5::util::iterator_adapter<T>::operator++: cannot increment end iterator"
      };
   }

   m_itor->to_next (); release_if_at_end (); return *this;
}

template <typename T> void iterator_adapter <T>::operator ++ (int) {
   ++*this;
}

template <typename T> T & iterator_adapter <T>::operator * () const {
   if (is_end_iterator ()) {
      throw std::out_of_range {
         "ml5::util::iterator_adapter<T>::operator*: cannot dereference end iterator"
      };
   }

   return m_itor->get_current ();
}

template <typename T> T * iterator_adapter <T>::operator -> () const {
   return &**this;
}

template <typename T> bool iterator_adapter <T>::is_end_iterator () const noexcept {
   return !m_itor || m_itor->is_at_end ();
}

template <typename T> void iterator_adapter <T>::release_if_at_end () noexcept {
   if (is_end_iterator ()) {
      m_itor.reset ();
   }
}

}   // namespace ml5::util
