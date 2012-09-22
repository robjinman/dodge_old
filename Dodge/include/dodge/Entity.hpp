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
#include <cmath>
#include "definitions.hpp"
#include "EEvent.hpp"
#include "EventManager.hpp"
#include "Primitive.hpp"
#include "rapidxml/rapidxml.hpp"
#include "Vec3f.hpp"


namespace Dodge {


class Entity;
typedef boost::shared_ptr<Entity> pEntity_t;


class Entity : public boost::enable_shared_from_this<Entity> {
   public:
      Entity(long type);
      Entity(long name, long type);
      Entity(const Entity& copy);
      Entity(const Entity& copy, long name);

      // Derived classes may need these
      virtual void addToWorld() {}
      virtual void removeFromWorld() {}

      virtual void onEvent(const EEvent* event) {}

      inline void setParent(Entity* parent);
      inline void addChild(pEntity_t child);
      inline void removeChild(pEntity_t child);
      inline Entity* getParent() const;
      inline const std::set<pEntity_t>& getChildren() const;

      inline void setTranslation(float32_t x, float32_t y, float32_t z);
      inline void setTranslation_x(float32_t x);
      inline void setTranslation_y(float32_t y);
      inline void setTranslation_z(float32_t z);
      inline void translate(float32_t x, float32_t y, float32_t z);
      inline void translate_x(float32_t x);
      inline void translate_y(float32_t y);
      inline void translate_z(float32_t z);
      inline void setRotation(float32_t deg);
      inline void rotate(float32_t deg);
      void rotate(float32_t deg, const Vec2f& pivot);

      // Relative to parent (parent's model space)
      inline Vec3f getTranslation() const;
      inline float32_t getRotation() const;

      // Relative to origin (world space)
      inline float32_t getRotation_abs() const;
      Vec3f getTranslation_abs() const;

      inline void setScale(float32_t s);
      inline void setScale(float32_t x, float32_t y);
      inline void scale(float32_t s);
      inline void scale(float32_t x, float32_t y);
      inline void setShape(std::unique_ptr<Primitive> shape);

      inline float32_t getWidth() const;
      inline float32_t getHeight() const;

      inline const Primitive& getShape() const;
      inline long getName() const;
      inline long getTypeName() const;
      inline const Vec2f& getScale() const;

      inline pEntity_t getSharedPtr();

      virtual void draw(const Vec2f& at) const = 0;
      virtual void update() {}

      virtual void assignData(const rapidxml::xml_node<>* data);

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      virtual ~Entity() {}

   protected:
      static EventManager m_eventManager;

   private:
      void recomputeShape() const;
      void deepCopy(const Entity& copy);

      long m_name;
      long m_type;
      Vec2f m_scale;

      Vec2f m_transl;
      float32_t m_z;    // Store z coord separately
      float32_t m_rot;

      std::unique_ptr<Primitive> m_srcShape;             // Bounding polygon/shape
      mutable std::unique_ptr<Primitive> m_transShape;   // m_srcShape after rotation and scaling
      mutable bool m_bTransShape;                        // True if m_transShape has been computed

      // These are computed from m_transShape
      mutable float32_t m_width;
      mutable float32_t m_height;

      Entity* m_parent;
      std::set<pEntity_t> m_children;

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
// Entity::setParent
//===========================================
inline void Entity::setParent(Entity* parent) {
   if (m_parent) m_parent->removeChild(shared_from_this());
   m_parent = parent;
   m_parent->m_children.insert(shared_from_this());
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
// Entity::setTranslation
//===========================================
inline void Entity::setTranslation(float32_t x, float32_t y, float32_t z) {
   m_transl = Vec2f(x, y);
   m_z = z;
}

//===========================================
// Entity::setTranslation_x
//===========================================
inline void Entity::setTranslation_x(float32_t x) {
   m_transl.x = x;
}

//===========================================
// Entity::setTranslation_y
//===========================================
inline void Entity::setTranslation_y(float32_t y) {
   m_transl.y = y;
}

//===========================================
// Entity::setTranslation_z
//===========================================
inline void Entity::setTranslation_z(float32_t z) {
   m_z = z;
}

//===========================================
// Entity::translate
//===========================================
inline void Entity::translate(float32_t x, float32_t y, float32_t z) {
   m_transl = m_transl + Vec2f(x, y);
   m_z += z;
}

//===========================================
// Entity::translate_x
//===========================================
inline void Entity::translate_x(float32_t x) {
   m_transl.x += x;
}

//===========================================
// Entity::translate_y
//===========================================
inline void Entity::translate_y(float32_t y) {
   m_transl.y += y;
}

//===========================================
// Entity::translate_z
//===========================================
inline void Entity::translate_z(float32_t z) {
   m_z += z;
}

//===========================================
// Entity::setRotation
//===========================================
inline void Entity::setRotation(float32_t deg) {
   m_rot = deg;
   m_bTransShape = false;
}

//===========================================
// Entity::rotate
//===========================================
inline void Entity::rotate(float32_t deg) {
   m_rot += deg;
   m_bTransShape = false;
}

//===========================================
// Entity::getTranslation
//===========================================
inline Vec3f Entity::getTranslation() const {
   return Vec3f(m_transl.x, m_transl.y, m_z);
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
// Entity::setShape
//===========================================
inline void Entity::setShape(std::unique_ptr<Primitive> shape) {
   m_srcShape = std::move(shape);
   m_bTransShape = false;
}

//===========================================
// Entity::setScale
//===========================================
inline void Entity::setScale(float32_t x, float32_t y) {
   m_scale.x = x;
   m_scale.y = y;
   m_bTransShape = false;
}

//===========================================
// Entity::setScale
//===========================================
inline void Entity::setScale(float32_t s) {
   m_scale.x = s;
   m_scale.y = s;
   m_bTransShape = false;
}

//===========================================
// Entity::scale
//===========================================
inline void Entity::scale(float32_t x, float32_t y) {
   m_scale.x *= x;
   m_scale.y *= y;
   m_bTransShape = false;
}

//===========================================
// Entity::scale
//===========================================
inline void Entity::scale(float32_t s) {
   m_scale.x *= s;
   m_scale.y *= s;
   m_bTransShape = false;
}

//===========================================
// Entity::getWidth
//===========================================
inline float32_t Entity::getWidth() const {
   if (!m_bTransShape) recomputeShape();

   return m_width;
}

//===========================================
// Entity::getHeight
//===========================================
inline float32_t Entity::getHeight() const {
   if (!m_bTransShape) recomputeShape();

   return m_height;
}

//===========================================
// Entity::getShape
//===========================================
inline const Primitive& Entity::getShape() const {
   if (!m_bTransShape) recomputeShape();

   return *m_transShape;
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
