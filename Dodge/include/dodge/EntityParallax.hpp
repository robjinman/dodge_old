/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ENTITY_PARALLAX_HPP__
#define __ENTITY_PARALLAX_HPP__


#include "Entity.hpp"


namespace Dodge {


class EntityParallax {
   public:
      EntityParallax(Entity* entity, const XmlNode data);
      EntityParallax(Entity* entity, const Vec2f& range);
      EntityParallax(Entity* entity, const EntityParallax& copy);

      virtual void assignData(const XmlNode data);
      virtual void update();
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

   private:
      void init();

      Entity* m_entity;
      Vec2f m_range;
      Vec2f m_pos;
};


}


#endif
