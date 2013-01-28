/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstdlib>
#include <StackAllocator.hpp>


namespace Dodge {


//===========================================
// StackAllocator::StackAllocator
//===========================================
StackAllocator::StackAllocator(size_t size)
   : m_size(size), m_parentTop(0), m_child(NULL) {

   m_bottom = m_top = new byte_t[size];
}

//===========================================
// StackAllocator::StackAllocator
//===========================================
StackAllocator::StackAllocator(marker_t parentTop, size_t size)
   : m_size(size), m_parentTop(parentTop), m_child(NULL) {

   m_bottom = m_top = new byte_t[size];
}

//===========================================
// StackAllocator::~StackAllocator
//===========================================
StackAllocator::~StackAllocator() {
   delete m_child;
   delete[] m_bottom;
}

//===========================================
// StackAllocator::alloc
//===========================================
void* StackAllocator::alloc(size_t size) {
   if (m_top + size > m_bottom + m_size) {
      if (!m_child)
         m_child = new StackAllocator(reinterpret_cast<marker_t>(m_bottom + m_size), m_size * 2);

      return m_child->alloc(size);
   }

   byte_t* p = m_top;
   m_top += size;
   return static_cast<void*>(p);
}

//===========================================
// StackAllocator::freeToMarker
//===========================================
void StackAllocator::freeToMarker(marker_t marker) {
   if (m_child) m_child->freeToMarker(marker);

   m_top = m_bottom + marker - m_parentTop;
   if (m_top < m_bottom) m_top = m_bottom;
}

//===========================================
// StackAllocator::getMarker
//===========================================
StackAllocator::marker_t StackAllocator::getMarker() const {
   if (m_child) return m_child->getMarker();

   return m_top - m_bottom + m_parentTop;
}

//===========================================
// StackAllocator::clear
//===========================================
void StackAllocator::clear() {
   if (m_child) m_child->clear();

   m_top = m_bottom;
}


}
