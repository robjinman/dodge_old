/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <WorldSpace.hpp>
#include <StringId.hpp>


namespace Dodge {


EventManager WorldSpace::m_eventManager;
std::unique_ptr<SpatialContainer<pEntity_t> > WorldSpace::m_container;
std::set<pEntity_t> WorldSpace::m_tracking;
bool WorldSpace::m_init = false;


//===========================================
// WorldSpace::entityMovedHandler
//===========================================
void WorldSpace::entityMovedHandler(EEvent* e) {
   assert(m_init);

   EEntityBoundingBox* event = static_cast<EEntityBoundingBox*>(e);

   std::set<pEntity_t>::iterator it = m_tracking.find(event->entity);
   if (it != m_tracking.end()) {
      m_container->remove(event->entity, event->oldBoundingBox);
      m_container->insert(event->entity, event->newBoundingBox);
   }
}


}
