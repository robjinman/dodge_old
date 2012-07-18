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
StackAllocator::StackAllocator(size_t size) : m_size(size) {
   m_bottom = m_top = new byte_t[size];
}

//===========================================
// StackAllocator::~StackAllocator
//===========================================
StackAllocator::~StackAllocator() {
   delete[] m_bottom;
}

//===========================================
// StackAllocator::alloc
//===========================================
void* StackAllocator::alloc(size_t size) {
   byte_t* p = m_top;

   if (m_top + size > m_bottom + m_size)
      return NULL;

   m_top += size;
   return static_cast<void*>(p);
}

//===========================================
// StackAllocator::freeToMarker
//===========================================
void StackAllocator::freeToMarker(marker_t marker) {
   m_top = m_bottom + marker;
}

//===========================================
// StackAllocator::getMarker
//===========================================
StackAllocator::marker_t StackAllocator::getMarker() const {
   return m_top - m_bottom;
}

//===========================================
// StackAllocator::clear
//===========================================
void StackAllocator::clear() {
   m_top = m_bottom;
}


}
