/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __PHYSICAL_SPRITE_HPP__
#define __PHYSICAL_SPRITE_HPP__


#include <boost/shared_ptr.hpp>
#include "renderer/Texture.hpp"
#include "Sprite.hpp"
#include "EntityPhysics.hpp"


namespace Dodge {


class PhysicalSprite : public Sprite, public EntityPhysics {
   public:
      PhysicalSprite(pEntityPhysicsImpl_t impl, long type, pTexture_t texture)
         : Sprite(type, texture), EntityPhysics(this, std::move(impl)) {}

      PhysicalSprite(pEntityPhysicsImpl_t impl, long name, long type, pTexture_t texture)
         : Sprite(name, type, texture), EntityPhysics(this, std::move(impl)) {}

      PhysicalSprite(const PhysicalSprite& copy)
         : Sprite(copy), EntityPhysics(copy, this) {}

      PhysicalSprite(const PhysicalSprite& copy, long name)
         : Sprite(copy, name), EntityPhysics(copy, this) {}

      virtual void addToWorld() {
         EntityPhysics::addToWorld();
      }

      virtual void removeFromWorld() {
         EntityPhysics::removeFromWorld();
      }

      virtual void update() {
         Sprite::update();
      }
};

typedef boost::shared_ptr<PhysicalSprite> pPhysicalSprite_t;


}


#endif
