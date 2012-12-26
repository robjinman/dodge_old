#ifndef __C_PARALLAX_SPRITE_HPP__
#define __C_PARALLAX_SPRITE_HPP__


#include <dodge/ParallaxSprite.hpp>
#include "Item.hpp"


class CParallaxSprite : public Item, public Dodge::ParallaxSprite {
   public:
      CParallaxSprite(const Dodge::XmlNode data);

      CParallaxSprite(const CParallaxSprite& copy)
         : Entity(copy), Item(copy), Dodge::ParallaxSprite(copy) {}

      CParallaxSprite(const CParallaxSprite& copy, long name)
         : Entity(copy, name), Item(copy, name), Dodge::ParallaxSprite(copy) {}

      virtual void update();

      virtual CParallaxSprite* clone() const;
      virtual void assignData(const Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
};


#endif
