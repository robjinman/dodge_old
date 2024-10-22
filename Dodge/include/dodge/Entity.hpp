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
#include "math/shapes/Shape.hpp"
#include "Range.hpp"
#include "renderer/Renderer.hpp"
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
      EEntityShape(pEntity_t entity_, pShape_t oldShape_, float32_t oldRotation_abs_,
         pShape_t newShape_, float32_t newRotation_abs_)
         : EEvent(internString("entityShape")), entity(entity_), oldShape(oldShape_),
           oldRotation_abs(oldRotation_abs_), newShape(newShape_), newRotation_abs(newRotation_abs_) {}

      pEntity_t entity;
      pShape_t oldShape;
      float32_t oldRotation_abs;
      pShape_t newShape;
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


class IAuxData {
   public:
      IAuxData() {}

      virtual size_t getSize() const = 0;
      virtual IAuxData* clone() const = 0;
      virtual void assignData(const XmlNode data) = 0;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const {}
#endif

      virtual ~IAuxData() {}
};


class Entity : virtual public Asset, virtual public boost::enable_shared_from_this<Entity> {
#ifdef DEBUG
   friend class Test;
#endif
   public:
      explicit Entity(const XmlNode data);
      explicit Entity(long type);
      Entity(long name, long type);
      Entity(const Entity& copy);
      Entity(const Entity& copy, long name);

      virtual size_t getSize() const;
      virtual Asset* clone() const;

      // The XML node needn't contain a complete description of the object.
      virtual void assignData(const XmlNode data);

      // Derived classes may need these
      virtual void addToWorld() {}
      virtual void removeFromWorld() {}

      virtual void onEvent(const EEvent* event) {}

      virtual void setParent(Entity* parent);
      virtual void addChild(pEntity_t child);
      virtual void removeChild(pEntity_t child);
      inline Entity* getParent() const;
      inline const std::set<pEntity_t>& getChildren() const;

      virtual void setSilent(bool b);
      inline bool isSilent() const;

      inline void attachAuxData(std::unique_ptr<IAuxData> data);
      inline IAuxData* getAuxDataPtr() const;

      void setTranslation_abs(float32_t x, float32_t y);
      inline void setTranslation_abs(const Vec2f& t);
      inline void setTranslation(float32_t x, float32_t y);
      inline void setTranslation(const Vec2f& t);
      inline void setTranslation_x(float32_t x);
      inline void setTranslation_y(float32_t y);
      virtual void translate(float32_t x, float32_t y);
      inline void translate(const Vec2f& t);
      inline void translate_x(float32_t x);
      inline void translate_y(float32_t y);
      virtual void setZ(float32_t z);
      inline void setRotation(float32_t deg);
      void setRotation_abs(float32_t deg);
      virtual void rotate(float32_t deg, const Vec2f& pivot = Vec2f(0.f, 0.f));
      inline void setScale(float32_t s);
      inline void setScale(float32_t x, float32_t y);
      inline void setScale(const Vec2f& s);
      inline void scale(float32_t s);
      virtual void scale(float32_t x, float32_t y);
      inline void scale(const Vec2f& s);
      virtual void setShape(std::unique_ptr<Shape> shape);

      // Relative to parent (parent's model space)
      inline Vec2f getTranslation() const;
      inline float32_t getZ() const;
      inline float32_t getRotation() const;

      // Relative to origin (world space)
      inline float32_t getRotation_abs() const;
      Vec2f getTranslation_abs() const;

      inline bool hasShape() const;
      inline const Shape& getShape() const;
      inline long getName() const;
      inline long getTypeName() const;
      inline const Vec2f& getScale() const;
      inline const Range& getBoundary() const;

      virtual void setFillColour(const Colour& colour);
      virtual void setLineColour(const Colour& colour);
      virtual void setLineWidth(Renderer::int_t lineWidth);
      virtual const Colour& getFillColour() const;
      virtual const Colour& getLineColour() const;
      virtual Renderer::int_t getLineWidth() const;

      inline pEntity_t getSharedPtr();

      virtual void draw() const;

      virtual void update() {}

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      virtual ~Entity();

   protected:
      static EventManager m_eventManager;

   private:
      void rotateShapes_r(float32_t deg);
      void recomputeBoundary();
      void deepCopy(const Entity& copy);
      void onParentTransformation(float32_t oldRot, const Vec2f& oldTransl);

      std::unique_ptr<IAuxData> m_auxData;

      long m_name;
      long m_type;
      bool m_silent; // If true, entity does not propagate any events
      Vec2f m_scale;

      Vec2f m_transl;
      float32_t m_z;
      float32_t m_rot;

      std::unique_ptr<Shape> m_shape; // Bounding polygon/shape
      Range m_boundary;

      Colour m_fillColour;
      Colour m_lineColour;
      Renderer::int_t m_lineWidth;

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
// Entity::attachAuxData
//===========================================
inline void Entity::attachAuxData(std::unique_ptr<IAuxData> data) {
   m_auxData = std::move(data);
}

//===========================================
// Entity::getAuxDataPtr
//===========================================
inline IAuxData* Entity::getAuxDataPtr() const {
   return m_auxData.get();
}

//===========================================
// Entity::isSilent
//===========================================
inline bool Entity::isSilent() const {
   return m_silent;
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
// Entity::setTranslation_abs
//===========================================
inline void Entity::setTranslation_abs(const Vec2f& t) {
   setTranslation_abs(t.x, t.y);
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
// Entity::getZ
//===========================================
inline float32_t Entity::getZ() const {
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
inline const Shape& Entity::getShape() const {
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
