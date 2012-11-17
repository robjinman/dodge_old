/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <Exception.hpp>
#include <EEvent.hpp>


using namespace std;


namespace Dodge {


StackAllocator EEvent::m_stack = StackAllocator(EEvent::STACK_SIZE);


//===========================================
// EEvent::operator new
//===========================================
void* EEvent::operator new(size_t size) {
   if (size == 0) size = 1;

   while (true) {
#ifdef DEFAULT_NEW
      void* p;
#else
      void* p = m_stack.alloc(size);
      if (p) return p;
#endif
      p = ::operator new(size);
      if (p) return p;

      new_handler globalHandler = set_new_handler(0);
      set_new_handler(globalHandler);

      if (globalHandler)
         (*globalHandler)();
      else
         throw std::bad_alloc();
   }
}

//===========================================
// EEvent::operator delete
//===========================================
void EEvent::operator delete(void* obj, size_t size) {
#ifdef DEFAULT_NEW
   ::operator delete(obj);
#endif
}


}
