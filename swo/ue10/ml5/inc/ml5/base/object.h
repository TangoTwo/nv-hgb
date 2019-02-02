//       $Id: object.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/base/object.h $
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

#include "../metainfo/metainfo_helper.h"

namespace ml5 {

class ML5_DLL_EXPORT object : MI5_NOBASE_DERIVE (object) {
   MI5_NOBASE_INJECT (object)

   public:
      using minfo_cont_t = metainfo_base::minfo_cont_t;

      object () = default;

      object (object const &) = delete;
      object (object &&) = delete;

      virtual ~object () = 0;

      object & operator = (object const &) = delete;
      object & operator = (object &&) = delete;

      std::string const &  get_class_name     () const noexcept;
      minfo_cont_t const & get_metainfo_bases () const noexcept;

      bool is_a (std::string const & name) const noexcept;
      bool is_a (std::type_info const & type) const noexcept;

      template <typename C> bool is_a () const noexcept {
         return get_metainfo ().is_a <C> ();
      }
};

}   // namespace ml5
