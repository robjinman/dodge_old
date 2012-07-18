/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __EVENT_MANAGER_HPP__
#define __EVENT_MANAGER_HPP__


#include <queue>
#include <map>
#include "utils/Functor.hpp"
#include "EEvent.hpp"


namespace Dodge {


class EventManager {
   public:
      inline void queueEvent(EEvent* event);
      inline void registerCallback(long type, const Functor<void, TYPELIST_1(EEvent*)>& func);
      void unregisterCallback(long type, const Functor<void, TYPELIST_1(EEvent*)>& func);
      void doEvents();
      inline void clear();

   private:
      static std::map<long, std::vector<Functor<void, TYPELIST_1(EEvent*)> > > m_callbacks;
      static std::queue<EEvent*> m_eventQueue;
};

//===========================================
// EventManager::queueEvent
//===========================================
inline void EventManager::queueEvent(EEvent* event) {
   m_eventQueue.push(event);
}

//===========================================
// EventManager::registerCallback
//===========================================
inline void EventManager::registerCallback(long type, const Functor<void, TYPELIST_1(EEvent*)>& func) {
   m_callbacks[type].push_back(func);
}

//===========================================
// EventManager::clear
//===========================================
inline void EventManager::clear() {
   while (!m_eventQueue.empty()) m_eventQueue.pop();
   EEvent::m_stack.clear();
}


}

#endif /*!__EVENT_MANAGER_HPP__*/
