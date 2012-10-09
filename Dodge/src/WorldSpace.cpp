/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <WorldSpace.hpp>
#include <StringId.hpp>
#include <EEntityMoved.hpp>


namespace Dodge {


EventManager WorldSpace::m_eventManager = EventManager();
std::unique_ptr<SpatialContainer<pEntity_t> > WorldSpace::m_container = std::unique_ptr<SpatialContainer<pEntity_t> >();
std::set<pEntity_t> WorldSpace::m_tracking = std::set<pEntity_t>();
bool WorldSpace::m_init = false;


//===========================================
// WorldSpace::entityMovedHandler
//===========================================
void WorldSpace::entityMovedHandler(EEvent* e) {
   if (!m_init)   // This is impossible
      throw Exception("Error in entityMoveHandler; WorldSpace not initialised", __FILE__, __LINE__);

   EEntityMoved* event = static_cast<EEntityMoved*>(e);

   std::set<pEntity_t>::iterator it = m_tracking.find(event->entity);
   if (it != m_tracking.end()) {
      m_container->remove(event->entity, event->oldBoundingBox);
      m_container->insert(event->entity, event->newBoundingBox);
   }
}


}
