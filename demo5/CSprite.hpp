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

      CSprite(const CSprite& copy)
         : Entity(copy),
           Item(copy),
           Sprite(copy) {}

      CSprite(const CSprite& copy, long name)
         : Entity(copy, name),
           Item(copy, name),
           Sprite(copy, name) {}

      virtual void render() const;
      virtual void unrender() const;

      virtual void onEvent(const Dodge::EEvent* event) { std::cout << "CSprite::onEvent()\n"; }
      virtual void update();
      virtual CSprite* clone() const;
      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void assignData(const Dodge::XmlNode data);

      virtual ~CSprite() {}
};

typedef boost::shared_ptr<CSprite> pCSprite_t;


#endif
