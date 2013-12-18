/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __PHYSICAL_SPRITE_HPP__
#define __PHYSICAL_SPRITE_HPP__


#include <memory>
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
      //===========================================
      // PhysicalSprite::PhysicalSprite
      //===========================================
      explicit PhysicalSprite(const XmlNode data)
         : Asset(internString("PhysicalSprite")),
           Entity(data.firstChild().firstChild()),
           Sprite(data.firstChild()),
           T_PHYSICS(this, data.nthChild(1)) {

         XML_NODE_CHECK(data, PhysicalSprite);
      }

      //===========================================
      // PhysicalSprite::PhysicalSprite
      //===========================================
      PhysicalSprite(long type, pTexture_t texture, std::unique_ptr<Shape> shape)
         : Asset(internString("PhysicalSprite")), Entity(type), Sprite(type, texture), T_PHYSICS(this) {

         setSilent(true);
         setShape(std::move(shape));
         setSilent(false);
      }

      //===========================================
      // PhysicalSprite::PhysicalSprite
      //===========================================
      PhysicalSprite(long name, long type, pTexture_t texture, std::unique_ptr<Shape> shape)
         : Asset(internString("PhysicalSprite")), Entity(name, type), Sprite(name, type, texture), T_PHYSICS(this) {

         setSilent(true);
         setShape(std::move(shape));
         setSilent(false);
      }

      //===========================================
      // PhysicalSprite::PhysicalSprite
      //===========================================
      PhysicalSprite(long type, pTexture_t texture, std::unique_ptr<Shape> shape, const EntityPhysics::options_t& options)
         : Asset(internString("PhysicalSprite")), Entity(type), Sprite(type, texture), T_PHYSICS(this, options) {

         setSilent(true);
         setShape(std::move(shape));
         setSilent(false);
      }

      //===========================================
      // PhysicalSprite::PhysicalSprite
      //===========================================
      PhysicalSprite(long name, long type, pTexture_t texture, std::unique_ptr<Shape> shape, const EntityPhysics::options_t& options)
         : Asset(internString("PhysicalSprite")), Entity(name, type), Sprite(name, type, texture), T_PHYSICS(this, options) {

         setSilent(true);
         setShape(std::move(shape));
         setSilent(false);
      }

      //===========================================
      // PhysicalSprite::PhysicalSprite
      //===========================================
      PhysicalSprite(const PhysicalSprite& copy)
         : Asset(internString("PhysicalSprite")), Entity(copy), Sprite(copy), T_PHYSICS(copy, this) {}

      //===========================================
      // PhysicalSprite::PhysicalSprite
      //===========================================
      PhysicalSprite(const PhysicalSprite& copy, long name)
         : Asset(internString("PhysicalSprite")), Entity(copy), Sprite(copy, name), T_PHYSICS(copy, this) {}

      //===========================================
      // PhysicalSprite::getSize
      //===========================================
      virtual size_t getSize() const {
         return Sprite::getSize() + T_PHYSICS::getSize();
      }

      //===========================================
      // PhysicalSprite::clone
      //===========================================
      virtual Asset* clone() const {
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

      //===========================================
      // PhysicalSprite::onEvent
      //===========================================
      virtual void onEvent(const EEvent* event) {
         T_PHYSICS::onEvent(event);
         Sprite::onEvent(event);
      }

      #ifdef DEBUG
      //===========================================
      // PhysicalSprite::dbg_print
      //===========================================
      virtual void dbg_print(std::ostream& out, int tab = 0) const {
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
         Sprite::addToWorld();
         T_PHYSICS::addToWorld();
      }

      //===========================================
      // PhysicalSprite::removeFromWorld
      //===========================================
      virtual void removeFromWorld() {
         Sprite::removeFromWorld();
         T_PHYSICS::removeFromWorld();
      }

      //===========================================
      // PhysicalSprite::update
      //===========================================
      virtual void update() {
         T_PHYSICS::update();
         Sprite::update();
      }

      virtual ~PhysicalSprite() {}
};


}


#endif
