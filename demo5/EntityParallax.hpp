#ifndef __ENTITY_PARALLAX_HPP__
#define __ENTITY_PARALLAX_HPP__


#include <dodge/Entity.hpp>


class EntityParallax {
   public:
      EntityParallax(Dodge::Entity* entity, const Dodge::XmlNode data);
      EntityParallax(Dodge::Entity* entity, const Dodge::Vec2f& range);
      EntityParallax(Dodge::Entity* entity, const EntityParallax& copy);

      virtual void assignData(const Dodge::XmlNode data);
      virtual void update();
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

   private:
      void init();

      Dodge::Entity* m_entity;
      Dodge::Vec2f m_range;
      Dodge::Vec2f m_pos;
};


#endif
