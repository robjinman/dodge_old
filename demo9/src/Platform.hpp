#ifndef __PLATFORM_HPP__
#define __PLATFORM_HPP__


#include <set>
#include <dodge/PhysicalEntity.hpp>
#include <dodge/Box2dPhysics.hpp>


class Platform : public Dodge::PhysicalEntity<Dodge::Box2dPhysics> {
   public:
      Platform(Dodge::float32_t w, Dodge::float32_t h);

      virtual void onEvent(const Dodge::EEvent* event);
      virtual void update();

      virtual ~Platform();

   private:
      std::set<Dodge::pEntity_t> m_contacts;
      std::set<Dodge::pEntity_t> m_pendingAddition;
      std::set<Dodge::pEntity_t> m_pendingRemoval;
};


#endif
