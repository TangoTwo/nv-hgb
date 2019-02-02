//       $Id: vector_iterator.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/container/vector_iterator.h $
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

namespace ml5 {

template <typename T> class vector_iterator final : public iterator <T>, MI5_DERIVE (vector_iterator <T>, iterator <T>) {
   MI5_INJECT (vector_iterator <T>)

   template <typename> friend class vector;

   public:
      T &  get_current () const override;
      bool is_at_end   () const noexcept override;
      void to_next     () override;

   private:
      explicit vector_iterator (T * ptr, std::size_t size);

      T * m_ptr {nullptr};
      T * m_end {nullptr};
};

}   // namespace ml5

// -------------------------------------------------------------------------------------------------

#include "./vector_iterator.inl"
