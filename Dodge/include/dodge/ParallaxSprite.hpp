/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __PARALLAX_SPRITE_HPP__
#define __PARALLAX_SPRITE_HPP__


#include "Sprite.hpp"
#include "EntityParallax.hpp"


namespace Dodge {


class ParallaxSprite : public Sprite, public EntityParallax {
   public:
      ParallaxSprite(const XmlNode data);
      ParallaxSprite(const ParallaxSprite& copy);
      ParallaxSprite(const ParallaxSprite& copy, long name);

      virtual void update();

      virtual size_t getSize() const;
      virtual Asset* clone() const;
      virtual void assignData(const XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
};


}


#endif
