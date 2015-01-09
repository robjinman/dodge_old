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

      EEvent(long type)
         : m_type(type), m_id(m_nextId++) {}

      inline long getType() const;
      inline long getId() const;

      static void* operator new(size_t size);
      static void operator delete(void* obj, size_t size);

      virtual ~EEvent() {};

   private:
      long m_type;
      long m_id;
      static long m_nextId;
      static StackAllocator m_stack;
};

//===========================================
// EEvent::getType
//===========================================
inline long EEvent::getType() const {
   return m_type;
}

//===========================================
// EEvent::getId
//===========================================
inline long EEvent::getId() const {
   return m_id;
}


}

#endif /*!__EEVENT_HPP__*/
