/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __PHYSICAL_SPRITE_HPP__
#define __PHYSICAL_SPRITE_HPP__


#include "renderer/Texture.hpp"
#include "Sprite.hpp"
#include "EntityPhysics.hpp"
#ifdef DEBUG
#include <ostream>
#endif
#include "xml/xml.hpp"


namespace Dodge {


template <class T_PHYSICS>
class PhysicalSprite : public Sprite, public T_PHYSICS {

   static_assert(
      std::is_base_of<EntityPhysics, T_PHYSICS>::value,
      "T_PHYSICS is not an implementation of EntityPhysics"
   );

   public:
      PhysicalSprite(const XmlNode data)
         : Sprite(data.nthChild(0)), T_PHYSICS(data.nthChild(1), this) {

         if (data.isNull() || data.name() != "PhysicalSprite")
            throw XmlException("Error parsing XML for instance of class PhysicalSprite; Expected 'PhysicalSprite' tag", __FILE__, __LINE__);
      }

      PhysicalSprite(long type, pTexture_t texture)
         : Sprite(type, texture), T_PHYSICS(this) {}

      PhysicalSprite(long name, long type, pTexture_t texture)
         : Sprite(name, type, texture), T_PHYSICS(this) {}

      PhysicalSprite(long type, pTexture_t texture, const EntityPhysics::options_t& options)
         : Sprite(type, texture), T_PHYSICS(this, options) {}

      PhysicalSprite(long name, long type, pTexture_t texture, const EntityPhysics::options_t& options)
         : Sprite(name, type, texture), T_PHYSICS(this, options) {}

      PhysicalSprite(const PhysicalSprite& copy)
         : Sprite(copy), T_PHYSICS(copy, this) {}

      PhysicalSprite(const PhysicalSprite& copy, long name)
         : Sprite(copy, name), T_PHYSICS(copy, this) {}

      //===========================================
      // PhysicalSprite::clone
      //===========================================
      virtual PhysicalSprite* clone() const {
         return new PhysicalSprite(*this);
      }

      //===========================================
      // PhysicalSprite::assignData
      //===========================================
      virtual void assignData(const XmlNode data) {
         if (data.isNull() || data.name() != "PhysicalSprite") return;

         XmlNode node = data.firstChild();

         if (!node.isNull() && node.name() == "Sprite") {
            Sprite::assignData(node);
            node = node.nextSibling();
         }

         if (!node.isNull() && node.name() == "EntityPhysics") {
            T_PHYSICS::assignData(data.nthChild(1));
         }
      }

      #ifdef DEBUG
      //===========================================
      // PhysicalSprite::dbg_print
      //===========================================
      virtual void dbg_print(std::ostream& out, int tab) const {
         for (int i = 0; i < tab; ++i) out << "\t";
         out << "PhysicalSprite:\n";
         Sprite::dbg_print(out, tab + 1);
         T_PHYSICS::dbg_print(out, tab + 1);
      }
      #endif

      //===========================================
      // PhysicalSprite::addToWorld
      //===========================================
      virtual void addToWorld() {
         T_PHYSICS::addToWorld();
      }

      //===========================================
      // PhysicalSprite::removeFromWorld
      //===========================================
      virtual void removeFromWorld() {
         T_PHYSICS::removeFromWorld();
      }

      //===========================================
      // PhysicalSprite::update
      //===========================================
      virtual void update() {
         Sprite::update();
      }
};


}


#endif
