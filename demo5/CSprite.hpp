#ifndef __C_SPRITE_HPP__
#define __C_SPRITE_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Sprite.hpp>
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class CSprite : public Item, public Dodge::Sprite {
   public:
      CSprite(const Dodge::XmlNode data);
      CSprite(const CSprite& copy);
      CSprite(const CSprite& copy, long name);

      virtual void draw() const;

      virtual void onEvent(const Dodge::EEvent* event) { Sprite::onEvent(event); }
      virtual void update();

      virtual size_t getSize() const;
      virtual CSprite* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(const Dodge::XmlNode data);

      virtual ~CSprite() {}
};

typedef boost::shared_ptr<CSprite> pCSprite_t;


#endif
