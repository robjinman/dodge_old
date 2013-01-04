/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <definitions.hpp>
#include <EventManager.hpp>


using namespace std;


namespace Dodge {


map<long, vector<Functor<void, TYPELIST_1(EEvent*)> > > EventManager::m_callbacks;
queue<EEvent*> EventManager::m_eventQueue;


//===========================================
// EventManager::doEvents
//===========================================
void EventManager::doEvents() {

   while (!m_eventQueue.empty()) {
      auto it = m_callbacks.find(m_eventQueue.front()->getType());

      if (it != m_callbacks.end()) {
         vector<Functor<void, TYPELIST_1(EEvent*)> >& funcs = it->second;

         for (uint_t i = 0; i < funcs.size(); ++i) {
            funcs[i](m_eventQueue.front());
            if (m_eventQueue.empty()) return; // In case funcs[i] calls EventManager::clear()
         }
      }

      delete m_eventQueue.front();
      m_eventQueue.pop();
   }

   EEvent::m_stack.clear();
}

//===========================================
// EventManager::immediateDispatch
//===========================================
void EventManager::immediateDispatch(EEvent* event) {
   auto it = m_callbacks.find(event->getType());

   if (it != m_callbacks.end()) {
      vector<Functor<void, TYPELIST_1(EEvent*)> >& funcs = it->second;
      for (uint_t i = 0; i < funcs.size(); ++i) funcs[i](event);
   }

   delete event;
}

//===========================================
// EventManager::clear
//===========================================
void EventManager::clear() {
   while (!m_eventQueue.empty()) m_eventQueue.pop();
   EEvent::m_stack.clear();
}

//===========================================
// EventManager::unregisterCallback
//===========================================
void EventManager::unregisterCallback(long type, const Functor<void, TYPELIST_1(EEvent*)>& func) {
   auto it = m_callbacks.find(type);

   if (it != m_callbacks.end()) {
      for (uint_t i = 0; i < it->second.size(); ++i) {

         if (it->second[i] == func) {
            it->second.erase(it->second.begin() + i);
            --i;
         }
      }
   }
}


}
