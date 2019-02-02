//       $Id: value.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/base/value.h $
// $Revision: 494 $
//     $Date: 2018-12-06 19:05:32 +0100 (Do., 06 Dez 2018) $
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

#include "./object.h"

// -------------------------------------------------------------------------------------------------

namespace ml5 {

template <typename T> class value final : public object, MI5_DERIVE (value <T>, object) {
   MI5_INJECT (value <T>)

   public:
      static_assert (std::is_nothrow_default_constructible_v <T>, "ml5::value<T>: T must be default constructible");
      static_assert (std::is_copy_constructible_v            <T>, "ml5::value<T>: T must be copy constructible");
      static_assert (std::is_nothrow_move_constructible_v    <T>, "ml5::value<T>: T must be move constructible");
      static_assert (std::is_nothrow_destructible_v          <T>, "ml5::value<T>: T must be destructible");
      static_assert (std::is_copy_assignable_v               <T>, "ml5::value<T>: T must be copy assignable");
      static_assert (std::is_nothrow_move_assignable_v       <T>, "ml5::value<T>: T must be move assignable");

      constexpr value () = default;

      value (value const &);
      value (value &&) noexcept;

      constexpr value (T) noexcept;

     ~value () = default;

      value & operator = (value const &);
      value & operator = (value &&) noexcept;

      value & operator = (T) noexcept;

      constexpr operator T &       () noexcept;
      constexpr operator T const & () const noexcept;

      friend bool operator == (value const & lhs, value const & rhs) noexcept {
         static_assert (
            util::is_equality_comparable_v <T>, "ml5::value<T>: T must be equality comparable"
         );

         return lhs.m_value == rhs.m_value;
      }

      friend bool operator < (value const & lhs, value const & rhs) noexcept {
         static_assert (
            util::is_less_than_comparable_v <T>, "ml5::value<T>: T must be less-than comparable"
         );

         return lhs.m_value < rhs.m_value;
      }

      friend bool operator != (value const & lhs, value const & rhs) noexcept {
         return !(lhs == rhs);
      }

      friend bool operator <= (value const & lhs, value const & rhs) noexcept {
         return !(rhs < lhs);
      }

      friend bool operator > (value const & lhs, value const & rhs) noexcept {
         return rhs < lhs;
      }

      friend bool operator >= (value const & lhs, value const & rhs) noexcept {
         return !(lhs < rhs);
      }

      T m_value {};
};

}   // namespace ml5

// -------------------------------------------------------------------------------------------------

namespace ml5 {

template <typename T> constexpr value <T> make_value (T v);

template <typename T> std::istream & operator >> (std::istream & lhs, value <T> & rhs);
template <typename T> std::ostream & operator << (std::ostream & lhs, value <T> const & rhs);

}   // namespace ml5

// -------------------------------------------------------------------------------------------------

namespace ml5::literals {

inline value <char> operator "" _v (char const v) {
   return value <char> {v};
}

inline value <std::string> operator "" _v (char const * const v, std::size_t) {
   return value <std::string> {v};
}

inline value <int> operator "" _v (unsigned long long int const v) {
   return value <int> (static_cast <int> (v));
}

inline value <double> operator "" _v (long double const v) {
   return value <double> (static_cast <double> (v));
}

}   // namespace ml5::literals

// -------------------------------------------------------------------------------------------------

#include "./value.inl"
