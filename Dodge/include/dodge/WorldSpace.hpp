/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __WORLD_SPACE_HPP__
#define __WORLD_SPACE_HPP__


#include <memory>
#include <set>
#include "EventManager.hpp"
#include "SpatialContainer.hpp"
#include "Entity.hpp"
#include "StringId.hpp"
#ifdef DEBUG
#include "renderer/Colour.hpp"
#endif


namespace Dodge {


class WorldSpace {
   public:
      void init(std::unique_ptr<SpatialContainer<pEntity_t> > container);

      void trackEntity(pEntity_t entity);
      void untrackEntity(pEntity_t entity);
      void untrackAll();
      void insertEntity(pEntity_t entity);
      void insertAndTrackEntity(pEntity_t entity);
      void removeEntity(pEntity_t entity);
      void removeAndUntrackEntity(pEntity_t entity);
      void removeAll();
      void removeAndUntrackAll();
      void getEntities(const Range& region, std::vector<pEntity_t>& entities) const;

#ifdef DEBUG
      void dbg_draw(const Colour& colour, Renderer::int_t lineWidth, float32_t z) const;
#endif

   private:
      static bool m_init;

      static EventManager m_eventManager;
      static std::unique_ptr<SpatialContainer<pEntity_t> > m_container;
      static std::set<pEntity_t> m_tracking;

      void entityMovedHandler(EEvent* e);
};


}


#endif
