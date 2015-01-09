/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2011
 */

#ifndef __STACK_ALLOCATOR_HPP__
#define __STACK_ALLOCATOR_HPP__


#include <cstring>
#include "definitions.hpp"


namespace Dodge {


class StackAllocator {
   public:
      typedef unsigned int marker_t;

      explicit StackAllocator(size_t size);
      ~StackAllocator();

      inline size_t getSize() const;
      void* alloc(size_t size);
      marker_t getMarker() const;
      void freeToMarker(marker_t marker);
      void clear();

   private:
      StackAllocator(marker_t parentTop, size_t size);

      size_t m_size;
      marker_t m_parentTop;
      byte_t* m_top;
      byte_t* m_bottom;

      StackAllocator* m_child;
};

//===========================================
// StackAllocator::getSize
//===========================================
size_t StackAllocator::getSize() const {
   return m_size;
}


}

#endif /*__STACK_ALLOCATOR_HPP__*/
