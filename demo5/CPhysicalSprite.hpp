#ifndef __C_PHYSICAL_SPRITE_HPP__
#define __C_PHYSICAL_SPRITE_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Box2dPhysics.hpp>
#include <dodge/PhysicalSprite.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class CPhysicalSprite : public Item, public Dodge::PhysicalSprite<Dodge::Box2dPhysics> {
   public:
      CPhysicalSprite(const Dodge::XmlNode data);
      CPhysicalSprite(const CPhysicalSprite& copy);
      CPhysicalSprite(const CPhysicalSprite& copy, long name);

      virtual void draw() const;
      virtual void update();

      virtual size_t getSize() const;
      virtual CPhysicalSprite* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(const Dodge::XmlNode data);

      virtual ~CPhysicalSprite() {}
};

typedef boost::shared_ptr<CPhysicalSprite> pCPhysicalSprite_t;


#endif
