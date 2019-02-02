//       $Id: macros.h 495 2018-12-06 18:07:06Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/utility/macros.h $
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

// -------------------------------------------------------------------------------------------------

#undef ML5_VERSION
#undef ML5_VERSION_MAYOR
#undef ML5_VERSION_MINOR

#define ML5_VERSION       (ML5_VERSION_MAYOR * 100 + ML5_VERSION_MINOR)
#define ML5_VERSION_MAYOR 5
#define ML5_VERSION_MINOR 0

// -------------------------------------------------------------------------------------------------

#undef  ML5_STRINGIZE
#define ML5_STRINGIZE(x) #x

#undef  ML5_STRINGIZE_MACRO
#define ML5_STRINGIZE_MACRO(x) ML5_STRINGIZE (x)

// -------------------------------------------------------------------------------------------------

#undef  MI5_DETAIL_NOBASE_INJECT_METHODS
#define MI5_DETAIL_NOBASE_INJECT_METHODS(C)                                        \
   public:                                                                         \
      static constexpr ml5::metainfo <C> const & get_metainfo_static () noexcept { \
         return ml5::get_metainfo <C> ();                                          \
      }                                                                            \
                                                                                   \
      virtual ml5::metainfo_base const & get_metainfo () const {                   \
         return get_metainfo_static ();                                            \
      }                                                                            \
                                                                                   \
   private:

#undef  MI5_DETAIL_INJECT_METHODS
#define MI5_DETAIL_INJECT_METHODS(C)                                               \
   public:                                                                         \
      static constexpr ml5::metainfo <C> const & get_metainfo_static () noexcept { \
         return ml5::get_metainfo <C> ();                                          \
      }                                                                            \
                                                                                   \
      ml5::metainfo_base const & get_metainfo () const override {                  \
         return get_metainfo_static ();                                            \
      }                                                                            \
                                                                                   \
   private:

// -------------------------------------------------------------------------------------------------

#define MI5_DERIVE(C, ...)   public ml5::util::metainfo_injector <C, false, __VA_ARGS__>
#define MI5_NOBASE_DERIVE(C) public ml5::util::metainfo_injector <C, false>

#define MI5_INJECT(C)        MI5_DETAIL_INJECT_METHODS (C)
#define MI5_NOBASE_INJECT(C) MI5_DETAIL_NOBASE_INJECT_METHODS (C)

#define MI5_INSTANTIATE(C, ...) (ml5::instantiate <C> (__VA_ARGS__))

// -------------------------------------------------------------------------------------------------

#define ML5_DEFINE_WIN_PROPERTY(name, type, init)         \
   protected:                                             \
      type const & set_prop_##name (type const & value) { \
         return m_prop_##name = value;                    \
      }                                                   \
                                                          \
      type const & get_prop_##name () const {             \
         return m_prop_##name;                            \
      }                                                   \
                                                          \
   private:                                               \
      type m_prop_##name {init};

// -------------------------------------------------------------------------------------------------

#define ML5_DECLARE_GET_WIN_PROPERTY(name, type) \
   type const & win_prop_##name () const;

#define ML5_DEFINE_GET_WIN_PROPERTY_DEF(class, name, type) \
   type const & class::win_prop_##name () const {          \
      return get_win ().get_prop_##name ();                \
   }

// -------------------------------------------------------------------------------------------------
