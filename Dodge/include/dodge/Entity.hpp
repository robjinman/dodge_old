/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ENTITY_HPP__
#define __ENTITY_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <set>
#include "definitions.hpp"
#include "StringId.hpp"
#include "EventManager.hpp"
#include "math/primitives/Primitive.hpp"
#include "Range.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/RenderBrush.hpp"
#include "xml/xml.hpp"
#include "Asset.hpp"


namespace Dodge {


class Entity;
typedef boost::shared_ptr<Entity> pEntity_t;


class EEntityBoundingBox : public EEvent {
   public:
      EEntityBoundingBox(pEntity_t entity_, const Range& oldBoundingBox_, const Range& newBoundingBox_)
         : EEvent(internString("entityBoundingBox")), entity(entity_), oldBoundingBox(oldBoundingBox_),
           newBoundingBox(newBoundingBox_) {}

      pEntity_t entity;
      Range oldBoundingBox;
      Range newBoundingBox;
};


class EEntityTranslation : public EEvent {
   public:
      EEntityTranslation(pEntity_t entity_, const Vec2f& oldTransl_, const Vec2f& oldTransl_abs_,
         const Vec2f& newTransl_, const Vec2f& newTransl_abs_)
         : EEvent(internString("entityTranslation")), entity(entity_), oldTransl(oldTransl_),
           oldTransl_abs(oldTransl_abs_), newTransl(newTransl_), newTransl_abs(newTransl_abs_) {}

      pEntity_t entity;
      Vec2f oldTransl;
      Vec2f oldTransl_abs;
      Vec2f newTransl;
      Vec2f newTransl_abs;
};


class EEntityShape : public EEvent {
   public:
      EEntityShape(pEntity_t entity_, pPrimitive_t oldShape_, float32_t oldRotation_abs_,
         pPrimitive_t newShape_, float32_t newRotation_abs_)
         : EEvent(internString("entityShape")), entity(entity_), oldShape(oldShape_),
           oldRotation_abs(oldRotation_abs_), newShape(newShape_), newRotation_abs(newRotation_abs_) {}

      pEntity_t entity;
      pPrimitive_t oldShape;
      float32_t oldRotation_abs;
      pPrimitive_t newShape;
      float32_t newRotation_abs;
};


class EEntityRotation : public EEvent {
   public:
      EEntityRotation(pEntity_t entity_, float32_t oldRotation_, float32_t oldRotation_abs_,
         float32_t newRotation_, float32_t newRotation_abs_)
            : EEvent(internString("entityRotation")), entity(entity_), oldRotation(oldRotation_),
              oldRotation_abs(oldRotation_abs_), newRotation(newRotation_), newRotation_abs(newRotation_abs_) {}

      pEntity_t entity;
      float32_t oldRotation;
      float32_t oldRotation_abs;
      float32_t newRotation;
      float32_t newRotation_abs;
};


class Entity : virtual public Asset, virtual public boost::enable_shared_from_this<Entity> {
   public:
      explicit Entity(const XmlNode data);
      explicit Entity(long type);
      Entity(long name, long type);
      Entity(const Entity& copy);
      Entity(const Entity& copy, long name);

      virtual Entity* clone() const = 0;

      // The XML node needn't contain a complete description of the object.
      virtual void assignData(const XmlNode data) = 0;

      // Derived classes may need these
      virtual void addToWorld() {}
      virtual void removeFromWorld() {}

      virtual void onEvent(const EEvent* event) {}

      void setParent(Entity* parent);
      inline void addChild(pEntity_t child);
      inline void removeChild(pEntity_t child);
      inline Entity* getParent() const;
      inline const std::set<pEntity_t>& getChildren() const;

      inline void setSilent(bool b);
      inline bool isSilent() const;

      inline void setTranslation(float32_t x, float32_t y);
      inline void setTranslation(const Vec2f& t);
      inline void setTranslation_x(float32_t x);
      inline void setTranslation_y(float32_t y);
      void translate(float32_t x, float32_t y);
      inline void translate(const Vec2f& t);
      inline void translate_x(float32_t x);
      inline void translate_y(float32_t y);
      inline void setZ(int z);
      inline void setRotation(float32_t deg);
      void rotate(float32_t deg, const Vec2f& pivot = Vec2f(0.f, 0.f));
      inline void setScale(float32_t s);
      inline void setScale(float32_t x, float32_t y);
      inline void setScale(const Vec2f& s);
      inline void scale(float32_t s);
      void scale(float32_t x, float32_t y);
      inline void scale(const Vec2f& s);
      void setShape(std::unique_ptr<Primitive> shape);

      inline void setRenderBrush(boost::shared_ptr<RenderBrush> brush);
      inline boost::shared_ptr<RenderBrush> getRenderBrush() const;

      // Relative to parent (parent's model space)
      inline Vec2f getTranslation() const;
      inline int getZ() const;
      inline float32_t getRotation() const;

      // Relative to origin (world space)
      inline float32_t getRotation_abs() const;
      Vec2f getTranslation_abs() const;

      inline bool hasShape() const;
      inline const Primitive& getShape() const;
      inline long getName() const;
      inline long getTypeName() const;
      inline const Vec2f& getScale() const;
      inline const Range& getBoundary() const;

      inline pEntity_t getSharedPtr();

      virtual void draw() const {}
      virtual void update() {}

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      virtual ~Entity() = 0;

   protected:
      static EventManager m_eventManager;

   private:
      void recomputeBoundary();
      void deepCopy(const Entity& copy);
      void parentMovedHandler();

      long m_name;
      long m_type;
      bool m_silent; // If true, entity does not propagate any events
      Vec2f m_scale;

      Vec2f m_transl;
      int m_z;
      float32_t m_rot;

      std::unique_ptr<Primitive> m_shape; // Bounding polygon/shape
      Range m_boundary;

      Entity* m_parent;
      std::set<pEntity_t> m_children;

      mutable boost::shared_ptr<RenderBrush> m_renderBrush;

      static int m_count;
      static long generateName();
};


//===========================================
// Entity::getSharedPtr
//===========================================
inline pEntity_t Entity::getSharedPtr() {
   return shared_from_this();
}

//===========================================
// Entity::setSilent
//===========================================
inline void Entity::setSilent(bool b) {
   m_silent = b;
}

//===========================================
// Entity::isSilent
//===========================================
inline bool Entity::isSilent() const {
   return m_silent;
}

//===========================================
// Entity::addChild
//===========================================
inline void Entity::addChild(pEntity_t child) {
   child->setParent(this);
}

//===========================================
// Entity::removeChild
//===========================================
inline void Entity::removeChild(pEntity_t child) {
   if (child->m_parent == this)
      child->m_parent = NULL;

   m_children.erase(child);
}

//===========================================
// Entity::getParent
//===========================================
inline Entity* Entity::getParent() const {
   return m_parent;
}

//===========================================
// Entity::getChildren
//===========================================
inline const std::set<pEntity_t>& Entity::getChildren() const {
   return m_children;
}

//===========================================
// Entity::translate
//===========================================
inline void Entity::translate(const Vec2f& t) {
   translate(t.x, t.y);
}

//===========================================
// Entity::setTranslation
//===========================================
inline void Entity::setTranslation(float32_t x, float32_t y) {
   translate(Vec2f(x, y) - m_transl);
}

//===========================================
// Entity::setTranslation
//===========================================
inline void Entity::setTranslation(const Vec2f& t) {
   setTranslation(t.x, t.y);
}

//===========================================
// Entity::setTranslation_x
//===========================================
inline void Entity::setTranslation_x(float32_t x) {
   setTranslation(x, getTranslation().y);
}

//===========================================
// Entity::setTranslation_y
//===========================================
inline void Entity::setTranslation_y(float32_t y) {
   setTranslation(getTranslation().x, y);
}

//===========================================
// Entity::translate_x
//===========================================
inline void Entity::translate_x(float32_t x) {
   translate(x, 0.f);
}

//===========================================
// Entity::translate_y
//===========================================
inline void Entity::translate_y(float32_t y) {
   translate(0.f, y);
}

//===========================================
// Entity::setScale
//===========================================
inline void Entity::setScale(float32_t s) {
   setScale(s, s);
}

//===========================================
// Entity::setScale
//===========================================
inline void Entity::setScale(float32_t x, float32_t y) {
   scale(x / m_scale.x, y / m_scale.y);
}

//===========================================
// Entity::scale
//===========================================
inline void Entity::scale(float32_t s) {
   scale(s, s);
}

//===========================================
// Entity::setScale
//===========================================
inline void Entity::setScale(const Vec2f& s) {
   setScale(s.x, s.y);
}

//===========================================
// Entity::scale
//===========================================
inline void Entity::scale(const Vec2f& s) {
   scale(s.x, s.y);
}

//===========================================
// Entity::setZ
//===========================================
inline void Entity::setZ(int z) {
   m_z = z;
}

//===========================================
// Entity::getZ
//===========================================
inline int Entity::getZ() const {
   return m_z;
}

//===========================================
// Entity::setRotation
//===========================================
inline void Entity::setRotation(float32_t deg) {
   rotate(deg - m_rot);
}

//===========================================
// Entity::getTranslation
//===========================================
inline Vec2f Entity::getTranslation() const {
   return m_transl;
}

//===========================================
// Entity::getRotation
//===========================================
inline float32_t Entity::getRotation() const {
   return m_rot;
}

//===========================================
// Entity::getRotation_abs
//===========================================
inline float32_t Entity::getRotation_abs() const {
   return m_parent ?
      m_parent->getRotation_abs() + m_rot : m_rot;
}

//===========================================
// Entity::setRenderBrush
//===========================================
inline void Entity::setRenderBrush(boost::shared_ptr<RenderBrush> brush) {
   m_renderBrush = brush;
}

//===========================================
// Entity::getRenderBrush
//===========================================
inline boost::shared_ptr<RenderBrush> Entity::getRenderBrush() const {
   if (!m_renderBrush)
      m_renderBrush = boost::shared_ptr<RenderBrush>(new RenderBrush);

   return m_renderBrush;
}

//===========================================
// Entity::getBoundary
//===========================================
inline const Range& Entity::getBoundary() const {
   return m_boundary;
}

//===========================================
// Entity::hasShape
//===========================================
inline bool Entity::hasShape() const {
   return m_shape ? true : false;
}

//===========================================
// Entity::getShape
//===========================================
inline const Primitive& Entity::getShape() const {
   if (!m_shape)
      throw Exception("Error returning entity's shape; Entity does not have a shape", __FILE__, __LINE__);

   return *m_shape;
}

//===========================================
// Entity::getName
//===========================================
inline long Entity::getName() const {
   return m_name;
}

//===========================================
// Entity::getTypeName
//===========================================
inline long Entity::getTypeName() const {
   return m_type;
}

//===========================================
// Entity::getScale
//===========================================
inline const Vec2f& Entity::getScale() const {
   return m_scale;
}


}


#endif /*!__ENTITY_HPP__*/
