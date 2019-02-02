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

// -------------------------------------------------------------------------------------------------

template <typename T> value <T>::value (value const & other) : value {other.m_value} {
}

template <typename T> value <T>::value (value && other) noexcept : value {std::move (other.m_value)} {
}

template <typename T> constexpr value <T>::value (T v) noexcept : m_value {std::move (v)} {
}

template <typename T> value <T> & value <T>::operator = (value const & rhs) {
   return *this = rhs.m_value;
}

template <typename T> value <T> & value <T>::operator = (value && rhs) noexcept {
   return *this = std::move (rhs.m_value);
}

template <typename T> value <T> & value <T>::operator = (T v) noexcept {
   m_value = std::move (v); return *this;
}

template <typename T> constexpr value <T>::operator T & () noexcept {
   return m_value;
}

template <typename T> constexpr value <T>::operator T const & () const noexcept {
   return m_value;
}

// -------------------------------------------------------------------------------------------------

template <typename T> constexpr value <T> make_value (T v) {
   return value <T> {std::move (v)};
}

template <typename T> std::istream & operator >> (std::istream & lhs, value <T> & rhs) {
   static_assert (
      util::is_istreamable_v <T>, "ml5::operator>>: T of ml5::value<T> must be readable from a std::istream"
   );

   return lhs >> rhs.m_value;
}

template <typename T> std::ostream & operator << (std::ostream & lhs, value <T> const & rhs) {
   static_assert (
      util::is_ostreamable_v <T>, "ml5::operator<<: T of ml5::value<T> must be writable to a std::ostream"
   );

   return lhs << rhs.m_value;
}

// -------------------------------------------------------------------------------------------------

}   // namespace ml5
