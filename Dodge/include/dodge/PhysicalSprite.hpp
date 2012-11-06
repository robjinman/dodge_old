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
#ifdef DEBUG
#include <ostream>
#include "rapidxml/rapidxml.hpp"
#endif
#include "xml.hpp"


namespace Dodge {


class PhysicalSprite : public Sprite, public EntityPhysics {
   public:
      PhysicalSprite(const rapidxml::xml_node<>* data)
         : Sprite(nthChild(data, 0)), EntityPhysics(nthChild(data, 1)) {}

      PhysicalSprite(pEntityPhysicsImpl_t impl, long type, pTexture_t texture)
         : Sprite(type, texture), EntityPhysics(this, std::move(impl)) {}

      PhysicalSprite(pEntityPhysicsImpl_t impl, long name, long type, pTexture_t texture)
         : Sprite(name, type, texture), EntityPhysics(this, std::move(impl)) {}

      PhysicalSprite(const PhysicalSprite& copy)
         : Sprite(copy), EntityPhysics(copy, this) {}

      PhysicalSprite(const PhysicalSprite& copy, long name)
         : Sprite(copy, name), EntityPhysics(copy, this) {}

      virtual PhysicalSprite* clone() const;
      virtual void assignData(const rapidxml::xml_node<>* data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual void addToWorld();
      virtual void removeFromWorld();
      virtual void update();
};

typedef boost::shared_ptr<PhysicalSprite> pPhysicalSprite_t;


}


#endif
