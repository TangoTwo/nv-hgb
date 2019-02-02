//       $Id: metainfo_base.cpp 501 2018-12-06 18:36:01Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/src/metainfo/metainfo_base.cpp $
// $Revision: 501 $
//     $Date: 2018-12-06 19:36:01 +0100 (Do., 06 Dez 2018) $
//   Creator: peter.kulczycki<AT>fh-hagenberg.at
//  Creation: June 1, 2018
//   $Author: p20068 $
// Copyright: (c) 2018 Peter Kulczycki (peter.kulczycki<AT>fh-hagenberg.at)
//            (c) 2018 Michael Hava (michael.hava<AT>fh-hagenberg.at)
//   License: This document contains proprietary information belonging to
//            University of Applied Sciences Upper Austria, Campus Hagenberg. It
//            is distributed under the Boost Software License, Version 1.0 (see
//            http://www.boost.org/LICENSE_1_0.txt).

#include "../../inc/ml5/metainfo/metainfo_base.h"

namespace ml5 {

metainfo_base::~metainfo_base () {
   delete m_p_name;
   delete m_p_bases;
}

bool metainfo_base::abstract () const noexcept {
   return m_abstract;
}

metainfo_base::minfo_cont_t const & metainfo_base::bases () const noexcept {
   return util::deref (m_p_bases);
}

std::string const & metainfo_base::name () const noexcept {
   return util::deref (m_p_name);
}

std::type_info const & metainfo_base::type () const noexcept {
   return m_type;
}

bool metainfo_base::is_a (std::string const & name) const noexcept {
   if (util::deref (m_p_name) == name) {
      return true;
   }

   for (auto const & base : util::deref (m_p_bases)) {
      if (util::unwrap (base).is_a (name)) {
         return true;
      }
   }

   return false;
}

bool metainfo_base::is_a (std::type_info const & type) const noexcept {
   return is_a (util::type_as_string (type));
}

void metainfo_base::count_construction () noexcept {
   m_max_living = std::max (m_max_living, ++m_ctor_count - m_dtor_count);
}

void metainfo_base::count_destruction () noexcept {
   ++m_dtor_count;
}

std::ostream & metainfo_base::write (std::ostream & os) const {
   return os << util::deref (m_p_name);
}

metainfo_base::metainfo_base (bool const abstract, std::type_info const & type, minfo_cont_t bases)
   : m_abstract {abstract}
   , m_p_bases  {new minfo_cont_t {std::move (bases)}}
   , m_p_name   {new std::string {util::type_as_string (type)}}
   , m_type     {type} {
}

}   // namespace ml5
