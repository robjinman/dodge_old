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

//===========================================
// WorldSpace::init
//===========================================
void WorldSpace::init(std::unique_ptr<SpatialContainer<pEntity_t> > container) {
   m_container = std::move(container);

   Functor<void, TYPELIST_1(EEvent*)> fEntMovedHandler(this, &WorldSpace::entityMovedHandler);
   m_eventManager.registerCallback(internString("entityBoundingBox"), fEntMovedHandler);

   m_init = true;
}

//===========================================
// WorldSpace::trackEntity
//===========================================
void WorldSpace::trackEntity(pEntity_t entity) {
   if (!m_init)
      throw Exception("Error tracking entity; WorldSpace not initialised", __FILE__, __LINE__);

   m_tracking.insert(entity);
}

//===========================================
// WorldSpace::untrackEntity
//===========================================
void WorldSpace::untrackEntity(pEntity_t entity) {
   if (m_init) m_tracking.erase(entity);
}

//===========================================
// WorldSpace::untrackAll
//===========================================
void WorldSpace::untrackAll() {
   if (m_init) m_tracking.clear();
}

//===========================================
// WorldSpace::insertEntity
//===========================================
void WorldSpace::insertEntity(pEntity_t entity) {
   if (!m_init)
      throw Exception("Error inserting entity; WorldSpace not initialised", __FILE__, __LINE__);

   m_container->insert(entity, entity->getBoundary());
}

//===========================================
// WorldSpace::insertAndTrackEntity
//===========================================
void WorldSpace::insertAndTrackEntity(pEntity_t entity) {
   insertEntity(entity);
   trackEntity(entity);
}

//===========================================
// WorldSpace::removeEntity
//===========================================
void WorldSpace::removeEntity(pEntity_t entity) {
   if (m_init) m_container->remove(entity, m_container->getBoundary());
}

//===========================================
// WorldSpace::removeAll
//===========================================
void WorldSpace::removeAll() {
   if (m_init) m_container->removeAll();
}

//===========================================
// WorldSpace::removeAndUntrackEntity
//===========================================
void WorldSpace::removeAndUntrackEntity(pEntity_t entity) {
   removeEntity(entity);
   untrackEntity(entity);
}

//===========================================
// WorldSpace::removeAndUntrackAll
//===========================================
void WorldSpace::removeAndUntrackAll() {
   removeAll();
   untrackAll();
}

//===========================================
// WorldSpace::getEntities
//===========================================
void WorldSpace::getEntities(const Range& region, std::vector<pEntity_t>& entities) const {
   if (!m_init)
      throw Exception("Error retrieving entities; WorldSpace not initialised", __FILE__, __LINE__);

   m_container->getEntries(region, entities);
}

#ifdef DEBUG
//===========================================
// WorldSpace::dbg_draw
//===========================================
void WorldSpace::dbg_draw(const Colour& colour, Renderer::int_t lineWidth, float32_t z) const {
   if (!m_init)
      throw Exception("Error drawing worldspace; WorldSpace not initialised", __FILE__, __LINE__);

   m_container->dbg_draw(colour, lineWidth, z);
}
#endif


}
