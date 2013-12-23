#ifndef __BLOCK_HPP__
#define __BLOCK_HPP__


#include <dodge/PhysicalEntity.hpp>
#include <dodge/Box2dPhysics.hpp>


class Block : public Dodge::PhysicalEntity<Dodge::Box2dPhysics> {
   public:
      Block();

      virtual void onEvent(const Dodge::EEvent* event);
      virtual ~Block() {}

   private:
      int m_contacts;
};


#endif
