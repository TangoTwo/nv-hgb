//       $Id: traits.h 495 2018-12-06 18:07:06Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/utility/traits.h $
// $Revision: 495 $
//     $Date: 2018-12-06 19:07:06 +0100 (Do., 06 Dez 2018) $
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

#include "./detection.h"

#include <iostream>
#include <type_traits>
#include <utility>

namespace ml5::util {

// -------------------------------------------------------------------------------------------------

template <typename T, typename U> using is_similar = std::is_same <   // taken from stevedewhurst.com
   std::decay_t <T>, std::decay_t <U>
>;

template <typename T, typename U> constexpr bool is_similar_v {is_similar <T, U>::value};

// -------------------------------------------------------------------------------------------------

template <typename T> using EqualityComparable = std::enable_if_t <
   std::is_convertible_v <
      decltype (std::declval <T> () == std::declval <T> ()), bool
   >
>;

template <typename T> using LessThanComparable = std::enable_if_t <
   std::is_convertible_v <
      decltype (std::declval <T> () < std::declval <T> ()), bool
   >
>;

template <typename T> using IStreamable = std::enable_if_t <
   is_similar_v <
      decltype (std::declval <std::istream> () >> std::declval <T> ()), std::istream /*&*/
   >
>;

template <typename T> using OStreamable = std::enable_if_t <
   is_similar_v <
      decltype (std::declval <std::ostream> () << std::declval <T> ()), std::ostream /*&*/
   >
>;

// -------------------------------------------------------------------------------------------------

template <typename T, typename = void> struct is_equality_comparable  final : std::false_type {};
template <typename T, typename = void> struct is_less_than_comparable final : std::false_type {};
template <typename T, typename = void> struct is_istreamable          final : std::false_type {};
template <typename T, typename = void> struct is_ostreamable          final : std::false_type {};

template <typename T> struct is_equality_comparable  <T, EqualityComparable <T>> final : std::true_type {};
template <typename T> struct is_less_than_comparable <T, LessThanComparable <T>> final : std::true_type {};
template <typename T> struct is_istreamable          <T, IStreamable        <T>> final : std::true_type {};
template <typename T> struct is_ostreamable          <T, OStreamable        <T>> final : std::true_type {};

template <typename T> constexpr bool is_equality_comparable_v  {is_equality_comparable  <T>::value};
template <typename T> constexpr bool is_less_than_comparable_v {is_less_than_comparable <T>::value};
template <typename T> constexpr bool is_istreamable_v          {is_istreamable          <T>::value};
template <typename T> constexpr bool is_ostreamable_v          {is_ostreamable          <T>::value};

// -------------------------------------------------------------------------------------------------

}   // namespace ml5::util
