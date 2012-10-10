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


namespace Dodge {


class Sprite
   : public Entity,
     public EntityAnimations,
     public EntityTransformations {

   public:
      Sprite(long type, pTexture_t texture)
         : Entity(type), EntityAnimations(this, texture), EntityTransformations(this) {}

      Sprite(long name, long type, pTexture_t texture)
         : Entity(name, type), EntityAnimations(this, texture), EntityTransformations(this) {}

      Sprite(const Sprite& sprite);

      Sprite(const Sprite& sprite, long name);

      virtual void draw(const Vec2f& at) const;
      virtual void update();
      virtual void assignData(const rapidxml::xml_node<>* data);

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
