//       $Id: iterator_adapter.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/container/iterator_adapter.h $
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

#include "./iterator.h"

// -------------------------------------------------------------------------------------------------

namespace ml5::util {

template <typename T> class iterator_adapter final {   // does not support *i++ (see https://en.cppreference.com/w/cpp/named_req/InputIterator)
   public:
      using difference_type   = std::ptrdiff_t;
      using iterator_category = std::input_iterator_tag;
      using pointer           = T *;
      using reference         = T &;
      using value_type        = std::remove_cv_t <T>;

      iterator_adapter () = default;

      explicit iterator_adapter (std::unique_ptr <iterator <T>>);

      bool               operator == (iterator_adapter const &) noexcept;
      bool               operator != (iterator_adapter const &) noexcept;
      iterator_adapter & operator ++ ();
      void               operator ++ (int);
      T &                operator *  () const;
      T *                operator -> () const;

   private:
      bool is_end_iterator   () const noexcept;
      void release_if_at_end () noexcept;

      std::shared_ptr <iterator <T>> m_itor;
};

}   // namespace ml5::util

// -------------------------------------------------------------------------------------------------

#include "./iterator_adapter.inl"
