//       $Id: libraries.h 495 2018-12-06 18:07:06Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/utility/libraries.h $
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

#undef ML5_HAVE_BOOST

// -------------------------------------------------------------------------------------------------

#undef ML5_HAVE_VLD
#undef ML5_VLD_HEADER_INCLUDED

#if __has_include (<vld.h>) && defined ML5_DO_USE_VLD   // Visual Leak Detector (https://kinddragon.github.io/vld)
   #include <vld.h>

   #define ML5_HAVE_VLD
   #define ML5_VLD_HEADER_INCLUDED
#endif

// -------------------------------------------------------------------------------------------------

#undef ML5_HAVE_GSL
#undef ML5_GSL_HEADER_INCLUDED

#if __has_include (<gsl/gsl>) && defined ML5_DO_USE_GSL   // Guideline Support Library (https://github.com/Microsoft/GSL)
   #include <gsl/gsl>

   #define ML5_HAVE_GSL
   #define ML5_GSL_HEADER_INCLUDED
#else
   #define Expects(c) assert (c)
#endif

// -------------------------------------------------------------------------------------------------

#undef ML5_HAVE_WINDOWS
#undef ML5_WINDOWS_HEADER_INCLUDED

#if defined ML5_DO_USE_WINDOWS && !defined _WINDOWS_ && __has_include (<windows.h>)
   #if __has_include (<wx/msw/wrapwin.h>)
      #include <wx/msw/wrapwin.h>   // include windows.h the way wxWidget does
   #else
      #include <windows.h>
   #endif
#endif

#if defined _WINDOWS_   // windows.h was included before this header, or here via wrapwin.h, or directly
   #define ML5_HAVE_WINDOWS
   #define ML5_WINDOWS_HEADER_INCLUDED
#endif

// -------------------------------------------------------------------------------------------------

#undef  ML5_DLL_EXPORT
#define ML5_DLL_EXPORT

#if defined ML5_DETECTED_COMPILER_CL
   #pragma comment (lib, "glfw3.lib")
   #pragma comment (lib, "glu32.lib")
   #pragma comment (lib, "opengl32.lib")
   #pragma comment (lib, "wxbase31ud.lib")
   #pragma comment (lib, "wxmsw31ud_core.lib")
   #pragma comment (lib, "wxmsw31ud_gl.lib")

   #undef  ML5_DLL_EXPORT
   #define ML5_DLL_EXPORT __declspec (dllexport)

#elif defined ML5_DETECTED_COMPILER_GCC
   #undef  ML5_DLL_EXPORT
   #define ML5_DLL_EXPORT __attribute__ ((dllexport))

#endif

// -------------------------------------------------------------------------------------------------
