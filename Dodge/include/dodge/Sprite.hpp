/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __SPRITE_HPP__
#define __SPRITE_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include <boost/shared_ptr.hpp>
#include "Entity.hpp"
#include "EntityAnimations.hpp"
#include "EntityTransformations.hpp"
#include "xml/xml.hpp"


namespace Dodge {


class Sprite
   : virtual public Entity,
     public EntityAnimations,
     public EntityTransformations {

   public:
      explicit Sprite(const XmlNode data);

      Sprite(long type, pTexture_t texture)
         : Entity(type), EntityAnimations(this, texture), EntityTransformations(this) {}

      Sprite(long name, long type, pTexture_t texture)
         : Entity(name, type), EntityAnimations(this, texture), EntityTransformations(this) {}

      Sprite(const Sprite& copy);

      Sprite(const Sprite& copy, long name);

      virtual Sprite* clone() const;
      virtual void assignData(const XmlNode data);

      virtual void onEvent(const EEvent* event);

      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void render() const;
      virtual void unrender() const;

      virtual void update();

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual ~Sprite() {}

   private:
      void deepCopy(const Sprite& copy);
};

typedef boost::shared_ptr<Sprite> pSprite_t;


}


#endif /*!__SPRITE_HPP__*/
