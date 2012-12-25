#ifndef __CAVE_HPP__
#define __CAVE_HPP__


#include "CSprite.hpp"
#include "EntityParallax.hpp"


class Cave : public CSprite, public EntityParallax {
   public:
      Cave(const Dodge::XmlNode data);

      Cave(const Cave& copy)
         : Entity(copy), CSprite(copy), EntityParallax(this, copy) {}

      Cave(const Cave& copy, long name)
         : Entity(copy, name), CSprite(copy, name), EntityParallax(this, copy) {}

      virtual void update();

      virtual Cave* clone() const;
      virtual void assignData(const Dodge::XmlNode data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
};


#endif
