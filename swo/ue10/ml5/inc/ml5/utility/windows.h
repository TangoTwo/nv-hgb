//       $Id: windows.h 495 2018-12-06 18:07:06Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/utility/windows.h $
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

#include "./utility.h"

namespace ml5::util {

#if defined ML5_WINDOWS_HEADER_INCLUDED
   inline std::string get_last_error_as_string () {
      LPSTR buffer {nullptr};

      FormatMessageA (
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
         nullptr,
         GetLastError (),
         MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
         reinterpret_cast <LPSTR> (&buffer),
         0,
         nullptr
      );

      std::string const message {buffer}; LocalFree (buffer); return message;
   }

   inline bool show_console () {
      return AllocConsole () != FALSE;
   }
#else
   inline std::string get_last_error_as_string () {
      return {};
   }

   constexpr bool show_console () {
      return true;
   }
#endif

}   // namespace ml5::util
