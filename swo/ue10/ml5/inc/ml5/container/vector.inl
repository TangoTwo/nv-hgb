//       $Id: integer.h 444 2018-07-31 19:00:26Z p20068 $
//      $URL: https://svn01.fh-hagenberg.at/se/minilib/ml5/product/ml5/Base/integer.h $
// $Revision: 444 $
//     $Date: 2018-07-31 21:00:26 +0200 (Di., 31 Jul 2018) $
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

namespace ml5 {

template <typename T> vector <T>::vector (vector const & other) {
   m_data = static_cast <T *> (std::calloc (m_capacity = other.m_capacity, sizeof (T)));

   if (!m_data)
      throw std::bad_alloc {};   // "ml5::vector<T>::vector: cannot allocate memory"

   try {
      for (; m_size < other.m_size; ++m_size)
         new (m_data + m_size) T {other.m_data[m_size]};

   } catch (...) {   // cleanup
      for (std::size_t i {0}; i < m_size; ++i)
         m_data[i].~T ();

      std::free (m_data); throw;
   }
}

template <typename T> vector <T>::vector (vector && other) noexcept {
   swap (other);
}

template <typename T> vector <T>::~vector () noexcept {
   clear ();
}

template <typename T> vector <T> & vector <T>::operator = (vector rhs) {
   swap (rhs); return *this;
}

template <typename T> T & vector <T>::operator [] (std::size_t const idx) {
   return at (idx);
}

template <typename T> T const & vector <T>::operator [] (std::size_t const idx) const {
   return at (idx);
}

template <typename T> void vector <T>::add (T value) {
   if (m_capacity <= m_size)
      grow ();

   new (m_data + m_size++) T {std::move (value)};
}

template <typename T> void vector <T>::clear () noexcept {
   for (std::size_t i {0}; i < m_size; ++i)
      m_data[i].~T ();

   std::free (m_data);

   m_capacity = 0;
   m_size     = 0;
   m_data     = nullptr;
}

template <typename T> bool vector <T>::contains (T const & value) const noexcept {
   return find (value) != m_size;
}

template <typename T> std::unique_ptr <iterator <T>> vector <T>::make_iterator () const {
   return std::unique_ptr <iterator <T>> {new vector_iterator <T> {m_data, m_size}};
}

template <typename T> void vector <T>::remove (T const & value) noexcept {
   if (std::size_t const idx {find (value)}; idx != m_size) {
      for (std::size_t i {idx + 1}; i < m_size; ++i)
         m_data[i - 1] = std::move (m_data[i]);

      m_data[m_size--].~T ();
   }
}

template <typename T> std::size_t vector <T>::size () const noexcept {
   return m_size;
}

template <typename T> T & vector <T>::at (std::size_t const idx) {
   if (idx >= m_size) {
      throw std::out_of_range {"ml5::vector<T>::at: index out of range"};
   }

   return m_data[idx];
}

template <typename T> T const & vector <T>::at (std::size_t const idx) const {
   return const_cast <vector <T> *> (this)->at (idx);
}

template <typename T> void vector <T>::swap (vector & other) noexcept {
   using std::swap;

   swap (m_capacity, other.m_capacity);
   swap (m_size,     other.m_size);
   swap (m_data,     other.m_data);
}

template <typename T> std::size_t vector <T>::find (T const & value) const noexcept {
   for (std::size_t i {0}; i < m_size; ++i)
      if (m_data[i] == value)
         return i;

   return m_size;
}

template <typename T> void vector <T>::grow (std::size_t new_capacity) {
   static_assert (std::is_nothrow_move_constructible_v <T>, "ml5::vector<T>::grow: T must be move constructible");
   static_assert (std::is_nothrow_destructible_v       <T>, "ml5::vector<T>::grow: T must be destructible");

   new_capacity = std::max ((new_capacity > 0) ? std::max (new_capacity, m_size) : m_size * 2, std::size_t {8});

   if (new_capacity > m_size) {
      T * const new_data {static_cast <T *> (std::calloc (new_capacity, sizeof (T)))};

      if (!new_data)
         throw std::bad_alloc {};   // "ml5::vector<T>::grow: cannot allocate memory"

      for (std::size_t i {0}; i < m_size; ++i) {
         new (new_data + i) T {std::move (m_data[i])}; m_data[i].~T ();
      }

      std::free (m_data);

      m_capacity = new_capacity;
      m_data     = new_data;
   }
}

}   // namespace ml5
