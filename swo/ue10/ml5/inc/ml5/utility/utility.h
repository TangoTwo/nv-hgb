//       $Id: utility.h 495 2018-12-06 18:07:06Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/utility/utility.h $
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

#include "./libraries.h"
#include "./constants.h"
#include "./traits.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <functional>
#include <initializer_list>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <optional>
#include <random>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#if defined ML5_DETECTED_COMPILER_GCC
   #include <cstdlib>
   #include <cxxabi.h>
#endif

namespace ml5 {

// -------------------------------------------------------------------------------------------------

template <typename T> T get_random_uniform (T const l, T const u) noexcept {
   static_assert (
      std::is_integral_v <T> || std::is_floating_point_v <T>,
      "ml5::get_random_uniform<T>: T must be an integral or floating-point type"
   );

   static std::mt19937_64 generator {
      static_cast <unsigned> (std::time (nullptr))   // !pwk: better use seed sequences
   };

   if constexpr (std::is_integral_v <T>) {
      return std::uniform_int_distribution  <T> {l, u} (generator);
   } else {
      return std::uniform_real_distribution <T> {l, u} (generator);
   }
}

// -------------------------------------------------------------------------------------------------

}   // namespace ml5

namespace ml5::util {

// -------------------------------------------------------------------------------------------------

template <typename F> class final_action final {   // taken from the Guideline Support Library
   public:
      explicit constexpr final_action (F fun) noexcept : m_fun {std::move (fun)} {
      }

      final_action (final_action const &) = delete;

      constexpr final_action (final_action && other) noexcept
         : m_fun    {std::move (other.m_fun)}
         , m_invoke {other.m_invoke} {

         other.m_invoke = false;
      }

      final_action & operator = (final_action const &) = delete;
      final_action & operator = (final_action &&) = delete;

     ~final_action () noexcept {
         if (m_invoke) {
            m_fun ();
         }
      }

   private:
      F    m_fun    {};
      bool m_invoke {true};
};

template <typename F> constexpr final_action <F> finally (F fun) noexcept {
   return final_action <F> (std::move (fun));
}

// -------------------------------------------------------------------------------------------------

template <typename T> constexpr std::type_info const & type_info () noexcept {
   return typeid (T);   // std::remove_cvref_t
}

template <typename T> constexpr std::type_info const & static_type_info (T const &) noexcept {
   return type_info <T> ();
}

// -------------------------------------------------------------------------------------------------

template <typename T> constexpr T * nnull (T * const p) noexcept {
   Expects (p); return p;
}

template <typename T> constexpr std::unique_ptr <T> & nnull (std::unique_ptr <T> & p) noexcept {
   Expects (p); return p;
}

template <typename T> constexpr std::unique_ptr <T> const & nnull (std::unique_ptr <T> const & p) noexcept {
   Expects (p); return p;
}

template <typename T> constexpr T & deref (T * const p) noexcept {
   return *nnull (p);
}

template <typename T> constexpr T & deref (std::unique_ptr <T> const & p) noexcept {
   return *nnull (p);
}

// -------------------------------------------------------------------------------------------------

template <typename T, typename ...A> T & deref_new (A && ...a) {
   return deref (new T {std::forward <A> (a)...});
}

template <template <typename ...> typename C, typename ...A> auto make_from_values (A && ...a) {
   using T = std::common_type_t <A...>; return C <T> {static_cast <T> (std::forward <A> (a))...};
}

template <typename T> constexpr std::unique_ptr <T> make_unique_ptr (T * const p) noexcept {
   return std::unique_ptr <T> {nnull (p)};
}

template <typename T> constexpr std::unique_ptr <T []> make_unique_ptr_to_carray (T * const p) noexcept {
   return std::unique_ptr <T []> {nnull (p)};
}

template <typename T, typename D> constexpr std::unique_ptr <T [], D> make_unique_ptr_to_carray (T * const p, D && d) noexcept {
   return std::unique_ptr <T [], D> {nnull (p), d};
}

// -------------------------------------------------------------------------------------------------

inline std::string normalize_type_name (std::string name) {
   using namespace std::string_view_literals;

   for (std::string_view const & pattern : {"class "sv, "enum "sv, "struct "sv}) {
      for (std::string::size_type pos {0}; pos != std::string::npos;) {
         if ((pos = name.find (pattern, pos)) != std::string::npos) {
            name.replace (pos, std::size (pattern), "");
         }
      }
   }

   for (std::string::size_type pos {0}; pos != std::string::npos;) {
      if ((pos = name.find ("> >", pos)) != std::string::npos) {
         name.replace (pos, 3, ">>");
      }
   }

   return name;
}

inline std::string type_as_string (std::type_info const & type) noexcept {
   #if defined ML5_HAVE_BOOST
      return normalize_type_name (boost::core::demangle (type.name ()));   // needs boost/core/typeinfo.hpp

   #elif defined ML5_DETECTED_COMPILER_GCC
      int        status {0};
      auto const name   {make_unique_ptr_to_carray (
         abi::__cxa_demangle (type.name (), nullptr, nullptr, &status),   // see https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
         std::free
      )};

      return normalize_type_name ((status == 0) ? name.get () : type.name ());

   #else
      return normalize_type_name (type.name ());
   #endif
}

template <typename T> std::string type_as_string () noexcept {
   return type_as_string (type_info <T> ());
}

template <typename T> std::string type_as_string (T const &) noexcept {
   return type_as_string (type_info <T> ());
}

// -------------------------------------------------------------------------------------------------

template <typename T> using wrapped_t = std::optional <std::reference_wrapper <T>>;

template <typename R> constexpr auto itor_from_ritor (R ritor) noexcept {
   return std::next (ritor).base ();
}

inline std::string remove_all (std::string s, char const c) {
   s.erase (std::remove (std::begin (s), std::end (s), c), std::end (s)); return s;
}

template <typename T> constexpr T & unwrap (wrapped_t <T> const & w) noexcept {
   return w.value ().get ();
}

// -------------------------------------------------------------------------------------------------

}   // namespace ml5::util
