//       $Id: container.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/container/container.h $
// $Revision: 494 $
//     $Date: 2018-12-06 19:05:32 +0100 (Do., 06 Dez 2018) $
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

#include "./iterator_adapter.h"

namespace ml5 {

// -------------------------------------------------------------------------------------------------

template <typename T> class container : public object, MI5_DERIVE (container <T>, object) {
   MI5_INJECT (container <T>)

   public:
      virtual ~container () = 0;

      virtual void                           add           (T value) = 0;
      virtual void                           clear         () noexcept = 0;
      virtual bool                           contains      (T const & value) const noexcept = 0;
      virtual std::unique_ptr <iterator <T>> make_iterator () const = 0;
      virtual void                           remove        (T const & value) noexcept = 0;
      virtual std::size_t                    size          () const noexcept = 0;

      util::iterator_adapter <T> begin () const noexcept;
      util::iterator_adapter <T> end   () const noexcept;

      bool           empty        () const noexcept;
      iterator <T> * new_iterator () const;
};

// -------------------------------------------------------------------------------------------------

template <typename T> std::ostream & operator << (std::ostream & lhs, container <T> const & rhs);

// -------------------------------------------------------------------------------------------------

}   // namespace ml5

#include "./container.inl"
