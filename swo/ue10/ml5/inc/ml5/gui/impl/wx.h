//       $Id: wx.h 534 2018-12-24 14:37:18Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/gui/impl/wx.h $
// $Revision: 534 $
//     $Date: 2018-12-24 15:37:18 +0100 (Mo., 24 Dez 2018) $
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

#include "../../utility/detection.h"

#if defined ML5_DETECTED_COMPILER_GCC
   #pragma GCC diagnostic push

   #pragma GCC diagnostic ignored "-Wcast-function-type"
   #pragma GCC diagnostic ignored "-Wconversion"
   #pragma GCC diagnostic ignored "-Wfloat-conversion"
   #pragma GCC diagnostic ignored "-Wignored-qualifiers"
   #pragma GCC diagnostic ignored "-Wpedantic"
#endif

#include <wx/wx.h>

#include <wx/dcbuffer.h>
#include <wx/dcgraph.h>
#include <wx/glcanvas.h>
#include <wx/graphics.h>

#if defined ML5_DETECTED_COMPILER_GCC
   #pragma GCC diagnostic pop
#endif

#include "../../utility/forwards.h"

// -------------------------------------------------------------------------------------------------

inline std::ostream & operator << (std::ostream & lhs, wxPoint const rhs) {
   return lhs << "{" << rhs.x << ',' << rhs.y << "}";
}

inline std::ostream & operator << (std::ostream & lhs, wxSize const rhs) {
   return lhs << "{" << rhs.x << ',' << rhs.y << "}";
}

// -------------------------------------------------------------------------------------------------

namespace ml5::util {

#if wxVERSION_NUMBER >= 3100

inline wxGLAttributes const & attributes () noexcept {
   static wxGLAttributes attributes {};
   static bool           init       {false};

   if (!init) {
      attributes.PlatformDefaults ().DoubleBuffer ().SampleBuffers (1).EndList (); init = true;
   }

   return attributes;
}

#endif

inline wxRect & normalize (wxRect & r) noexcept {
   if (r.width  < 0) { r.x += r.width;  r.width  *= -1; }
   if (r.height < 0) { r.y += r.height; r.height *= -1; }

   return r;
}

inline bool contains (wxRect r, wxPoint const p) noexcept {
   return normalize (r).Contains (p);
}

inline bool intersect (wxRect const a, wxRect const b) noexcept {
   return a.Intersects (b);
}

inline wxCursor make_blank_cursor () {
   constexpr int size {4};   // width/height of the cursor [pels]

   static bool init {false};

   static char bits [size * size];
   static char mask [size * size];

   if (!init) {
      std::memset (bits, 0xff, std::size (bits));
      std::memset (mask, 0xff, std::size (mask));

      init = true;
   }

   #if defined __WXMSW__
      wxBitmap       bits_bmp {bits, size, size};
      wxBitmap const mask_bmp {mask, size, size};

      bits_bmp.SetMask (new wxMask {mask_bmp});   // who is the owner of this wxMask?

      wxImage bits_img {bits_bmp.ConvertToImage ()};

      bits_img.SetOption (wxIMAGE_OPTION_CUR_HOTSPOT_X, 0);
      bits_img.SetOption (wxIMAGE_OPTION_CUR_HOTSPOT_Y, 0);

      wxCursor const cursor {bits_img};

   #elif defined __WXGTK__
      wxCursor const cursor {bits, size, size, 0, 0, mask, wxWHITE, wxBLACK};
   #endif

   return cursor;
}

}   // namespace ml5::util

// -------------------------------------------------------------------------------------------------
