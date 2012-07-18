/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __EEVENT_HPP__
#define __EEVENT_HPP__


#include "definitions.hpp"
#include "StackAllocator.hpp"


namespace Dodge {


class EEvent {
   friend class EventManager;

   public:
      static const int STACK_SIZE = 102400; // 100KB

      EEvent(long type) : m_type(type) {}

      inline long getType() const;

      static void* operator new(size_t size);
      static void operator delete(void* obj, size_t size);

      virtual ~EEvent() {};

   private:
      long m_type;
      static StackAllocator m_stack;
};

//===========================================
// EEvent::getType
//===========================================
inline long EEvent::getType() const {
   return m_type;
}


}

#endif /*!__EEVENT_HPP__*/
