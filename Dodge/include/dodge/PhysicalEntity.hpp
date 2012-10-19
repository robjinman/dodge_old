#ifndef __PHYSICAL_ENTITY_HPP__
#define __PHYSICAL_ENTITY_HPP__


class PhysicalEntity : public Entity, public EntityPhysics {
   public
      PhysicalEntity(long type)
         : Entity(type), EntityPhysics(this) {}

      PhysicalEntity(long name, long type)
         : Entity(name, type), EntityPhysics(this) {}

      PhysicalEntity(long type, const PhysicalEntity& copy)
         : Entity(type, copy), EntityPhysics(copy) {}

      PhysicalEntity(long name, long type, const PhysicalEntity& copy)
         : Entity(name, type), EntityPhysics(this) {}

      virtual void update() {
         Entity::update();
         EntityPhysics::update();
      }
};


#endif
