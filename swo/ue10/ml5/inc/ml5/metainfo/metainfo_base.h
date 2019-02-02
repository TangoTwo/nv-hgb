//       $Id: metainfo_base.h 494 2018-12-06 18:05:32Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/inc/ml5/metainfo/metainfo_base.h $
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

#include "../utility/forwards.h"

namespace ml5 {

// -------------------------------------------------------------------------------------------------

class ML5_DLL_EXPORT metainfo_base {
   template <typename> friend struct metainfo;

   public:
      using minfo_cont_t = std::vector <util::wrapped_t <metainfo_base const>>;

      metainfo_base (metainfo_base const &) = delete;
      metainfo_base (metainfo_base &&) = delete;

     ~metainfo_base ();

      metainfo_base & operator = (metainfo_base const &) = delete;
      metainfo_base & operator = (metainfo_base &&) = delete;

      bool                   abstract () const noexcept;
      minfo_cont_t const &   bases    () const noexcept;
      std::string const &    name     () const noexcept;
      std::type_info const & type     () const noexcept;

      bool is_a (std::string const & name) const noexcept;
      bool is_a (std::type_info const & type) const noexcept;

      template <typename C> bool is_a () const noexcept {
         return is_a (util::type_as_string <C> ());
      }

      void count_construction () noexcept;
      void count_destruction  () noexcept;

      std::ostream & write (std::ostream & os = std::cout) const;

   private:
      explicit metainfo_base (bool abstract, std::type_info const & type, minfo_cont_t bases);

      bool                   m_abstract   {false};
      std::size_t            m_ctor_count {0};
      std::size_t            m_dtor_count {0};
      std::size_t            m_max_living {0};
      minfo_cont_t *         m_p_bases    {nullptr};   // owning pointer (because of DLL issues)
      std::string *          m_p_name     {nullptr};   // owning pointer (because of DLL issues)
      std::type_info const & m_type       {util::type_info <void> ()};
};

// -------------------------------------------------------------------------------------------------

inline std::ostream & operator << (std::ostream & lhs, metainfo_base const & rhs) {
   return rhs.write (lhs);
}

// -------------------------------------------------------------------------------------------------

}   // namespace ml5
