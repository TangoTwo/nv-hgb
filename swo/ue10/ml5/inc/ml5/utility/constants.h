//       $Id: constants.h 509 2018-12-07 15:22:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/utility/constants.h $
// $Revision: 509 $
//     $Date: 2018-12-07 16:22:32 +0100 (Fr., 07 Dez 2018) $
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

// -------------------------------------------------------------------------------------------------

constexpr int version       {ML5_VERSION};
constexpr int version_mayor {ML5_VERSION_MAYOR};
constexpr int version_minor {ML5_VERSION_MINOR};

// -------------------------------------------------------------------------------------------------

namespace ml5::util {

template <typename T> constexpr T pi {static_cast <T> (3.1415926535897932384626433832795L)};

template <typename T> constexpr T deg {pi <T> / 180};
template <typename T> constexpr T rad {1 / deg <T>};

constexpr double DEG {deg <double>};
constexpr double PI  {pi <double>};
constexpr double RAD {rad <double>};

}   // namespace ml5::util

// -------------------------------------------------------------------------------------------------

namespace ml5::literals {

inline double operator "" _deg (unsigned long long int const value) {
   return static_cast <double> (value) * util::deg <double>;
}

inline double operator "" _deg (long double const value) {
   return static_cast <double> (value * util::deg <long double>);
}

inline double operator "" _rad (unsigned long long int const value) {
   return static_cast <double> (value) * util::rad <double>;
}

inline double operator "" _rad (long double const value) {
   return static_cast <double> (value * util::rad <long double>);
}

}   // namespace ml5::literals

// -------------------------------------------------------------------------------------------------
