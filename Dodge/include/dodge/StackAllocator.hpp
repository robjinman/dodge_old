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

      StackAllocator(size_t size);
      ~StackAllocator();

      inline size_t getSize() const;
      void* alloc(size_t size);
      marker_t getMarker() const;
      void freeToMarker(marker_t marker);
      void clear();

   private:
      size_t m_size;
      byte_t* m_top;
      byte_t* m_bottom;
};

//===========================================
// StackAllocator::getSize
//===========================================
size_t StackAllocator::getSize() const {
   return m_size;
}


}

#endif /*__STACK_ALLOCATOR_HPP__*/
