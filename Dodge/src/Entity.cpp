/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <sstream>
#include <cstdio>
#include <Entity.hpp>
#include <Exception.hpp>
#include <StringId.hpp>
#include <AssetManager.hpp>
#include <globals.hpp>
#include <PrimitiveFactory.hpp>


using namespace std;


namespace Dodge {


EventManager Entity::m_eventManager = EventManager();
int Entity::m_count = 0;


#ifdef DEBUG
//===========================================
// Entity::dbg_print
//===========================================
void Entity::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "Entity\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "name: " << getInternedString(m_name) << "\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "type: " << getInternedString(m_type) << "\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "scale: (" << m_scale.x << ", " << m_scale.y << ")\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "translation: (" << m_transl.x << ", " << m_transl.y << ")\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "z: " << m_z << "\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "rotation: " << m_rot << "\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "shape (src):\n";
   if (m_shape) m_shape->dbg_print(out, tab + 1);

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "boundary:\n";
   m_boundary.dbg_print(out, tab + 1);

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "parent: " << (m_parent ? getInternedString(m_parent->getName()) : "NULL") << "\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "children: ";
   if (m_children.size() > 0) {
      out << "\n";
      for (set<pEntity_t>::const_iterator c = m_children.begin(); c != m_children.end(); ++c) {
         for (int i = 0; i < tab + 2; ++i) out << "\t";
         out << getInternedString((*c)->getName()) << "\n";
      }
   }
   else
      out << "(none)\n";
}
#endif

//===========================================
// Entity::Entity
//===========================================
Entity::Entity(const XmlNode data)
   : m_silent(false) {

   AssetManager assetManager;
   PrimitiveFactory primitiveFactory;

   string msg("Error parsing XML for instance of class Entity");

   XML_NODE_CHECK(msg, data, Entity);

   XmlAttribute attr = data.firstAttribute();
   XML_ATTR_CHECK(msg, attr, type);
   m_type = internString(attr.value());

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, name);
   m_name = internString(attr.value());

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, x);
   sscanf(attr.value().data(), "%f", &m_transl.x);

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, y);
   sscanf(attr.value().data(), "%f", &m_transl.y);

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, z);
   sscanf(attr.value().data(), "%d", &m_z);

   attr = attr.nextAttribute();
   XML_ATTR_CHECK(msg, attr, rot);
   sscanf(attr.value().data(), "%f", &m_rot);

   XmlNode node = data.firstChild();
   XML_NODE_CHECK(msg, node, scale);
   m_scale = Vec2f(node.firstChild());

   node = node.nextSibling();
   XML_NODE_CHECK(msg, node, shape);
   m_shape = unique_ptr<Primitive>(primitiveFactory.create(node.firstChild()));

   node = node.nextSibling();
   XML_NODE_CHECK(msg, node, children);

   XmlNode node_ = node.firstChild();
   while (!node_.isNull() && node_.name() == "child") {
      XmlAttribute attr = node_.firstAttribute();

      if (!attr.isNull() && attr.name() == "ptr") {
         long id = 0;
         sscanf(attr.value().data(), "%ld", &id);

         pEntity_t child = boost::dynamic_pointer_cast<Entity>(assetManager.getAssetPointer(id));

         if (!child)
            throw XmlException(msg + "; Bad entity asset id", __FILE__, __LINE__);

         addChild(child);
      }

      node_ = node_.nextSibling();
   }

   ++m_count;
}

//===========================================
// Entity::Entity
//===========================================
Entity::Entity(long name, long type)
   : m_name(name), m_type(type), m_silent(false), m_scale(1.f, 1.f), m_transl(0.f, 0.f),
     m_z(1), m_rot(0.f), m_parent(NULL) {

   ++m_count;
}

//===========================================
// Entity::Entity
//===========================================
Entity::Entity(long type)
   : m_type(type), m_silent(false), m_scale(1.f, 1.f), m_transl(0.f, 0.f), m_z(1),
     m_rot(0.f), m_parent(NULL) {

   m_name = generateName();

   ++m_count;
}

//===========================================
// Entity::Entity
//
// Construct deep copy.
//===========================================
Entity::Entity(const Entity& copy) : m_parent(NULL) {
   deepCopy(copy);
   m_name = generateName();

   ++m_count;
}

//===========================================
// Entity::Entity
//
// Construct deep copy.
//===========================================
Entity::Entity(const Entity& copy, long name) : m_parent(NULL) {
   deepCopy(copy);
   m_name = name;

   ++m_count;
}

//===========================================
// Entity::deepCopy
//
// Copy's 'family' is not inherited
//===========================================
void Entity::deepCopy(const Entity& copy) {
   m_name = copy.m_name;
   m_type = copy.m_type;
   m_silent = copy.m_silent;
   m_scale = copy.m_scale;
   m_transl = copy.m_transl;
   m_z = copy.m_z;
   m_rot = copy.m_rot;

   m_shape = copy.m_shape ? unique_ptr<Primitive>(copy.m_shape->clone()) : unique_ptr<Primitive>();
   m_boundary = copy.m_boundary;
}

//===========================================
// Entity::generateName
//===========================================
long Entity::generateName() {
   stringstream str;
   str << "entity" << m_count;
   return internString(str.str());
}

//===========================================
// Entity::assignData
//===========================================
void Entity::assignData(const XmlNode data) {
   if (data.isNull() || data.name() != "Entity") return;

   AssetManager assetManager;
   PrimitiveFactory primitiveFactory;

   string msg("Error parsing XML for instance of class Entity");

   XmlAttribute attr = data.firstAttribute();
   if (!attr.isNull() && attr.name() == "type") {
      m_type = internString(attr.value());
      attr = attr.nextAttribute();
   }
   if (!attr.isNull() && attr.name() == "name") {
      m_name = internString(attr.value());
      attr = attr.nextAttribute();
   }
   if (!attr.isNull() && attr.name() == "x") {
      sscanf(attr.value().data(), "%f", &m_transl.x);
      attr = attr.nextAttribute();
   }
   if (!attr.isNull() && attr.name() == "y") {
      sscanf(attr.value().data(), "%f", &m_transl.y);
      attr = attr.nextAttribute();
   }
   if (!attr.isNull() && attr.name() == "z") {
      sscanf(attr.value().data(), "%d", &m_z);
      attr = attr.nextAttribute();
   }
   if (!attr.isNull() && attr.name() == "rot") {
      sscanf(attr.value().data(), "%f", &m_rot);
   }
   XmlNode node = data.firstChild();
   if (!node.isNull() && node.name() == "scale") {
      m_scale = Vec2f(node.firstChild());
      node = node.nextSibling();
   }
   if (!node.isNull() && node.name() == "shape") {
      m_shape = unique_ptr<Primitive>(primitiveFactory.create(node.firstChild()));
      node = node.nextSibling();
   }
   if (!node.isNull() && node.name() == "children") {
      XmlNode node_ = node.firstChild();

      while (!node_.isNull() && node_.name() == "child") {
         XmlAttribute attr = node_.firstAttribute();

         if (!attr.isNull() && attr.name() == "ptr") {
            long id = 0;
            sscanf(attr.value().data(), "%ld", &id);

            pEntity_t child = boost::dynamic_pointer_cast<Entity>(assetManager.getAssetPointer(id));

            if (!child)
               throw XmlException(msg + "; Bad entity asset id", __FILE__, __LINE__);

            addChild(child);
         }

         node_ = node_.nextSibling();
      }
   }
}

//===========================================
// Entity::setParent
//===========================================
void Entity::setParent(Entity* parent) {
   if (m_parent) m_parent->removeChild(shared_from_this());
   m_parent = parent;
   m_parent->m_children.insert(shared_from_this());

   Range bounds = m_boundary;
   recomputeBoundary();

   if (!m_silent) {
      EEvent* event = new EEntityBoundingBox(shared_from_this(), bounds, m_boundary);
      onEvent(event);
      m_eventManager.queueEvent(event);
   }

   // Children must dispatch EEntityBoundingBox events too
   for (set<pEntity_t>::iterator i = m_children.begin(); i != m_children.end(); ++i)
      (*i)->parentMovedHandler();
}

//===========================================
// Entity::getTranslation_abs
//===========================================
Vec2f Entity::getTranslation_abs() const {
   if (m_parent) {
      float32_t x = m_transl.x * cos(DEG_TO_RAD(m_parent->getRotation_abs()))
         - m_transl.y * sin(DEG_TO_RAD(m_parent->getRotation_abs()));

      float32_t y = m_transl.x * sin(DEG_TO_RAD(m_parent->getRotation_abs()))
         + m_transl.y * cos(DEG_TO_RAD(m_parent->getRotation_abs()));

      Vec2f t = m_parent->getTranslation_abs();

      return t + Vec2f(x, y);
   }
   else {
      return getTranslation();
   }
}

//===========================================
// Entity::recomputeBoundary
//===========================================
void Entity::recomputeBoundary() {
   Vec2f min = m_shape ? m_shape->getMinimum() : Vec2f(0.f, 0.f);
   Vec2f max = m_shape ? m_shape->getMaximum() : Vec2f(0.f, 0.f);

   m_boundary.setPosition(getTranslation_abs() + min);
   m_boundary.setSize(max - min);
}

//===========================================
// Entity::translate
//===========================================
void Entity::translate(float32_t x, float32_t y) {
   Range bounds = m_boundary;

   m_transl = m_transl + Vec2f(x, y);
   m_boundary.setPosition(m_boundary.getPosition() + Vec2f(x, y));

   if (!m_silent) {
      EEvent* event = new EEntityBoundingBox(shared_from_this(), bounds, m_boundary);
      onEvent(event);
      m_eventManager.queueEvent(event);
   }

   // Children must dispatch EEntityBoundingBox events too
   for (set<pEntity_t>::iterator i = m_children.begin(); i != m_children.end(); ++i)
      (*i)->parentMovedHandler();
}

//===========================================
// Entity::rotate
//
// Pivot is in model space
//===========================================
void Entity::rotate(float32_t deg, const Vec2f& pivot) {
   float32_t oldRot = m_rot;
   float32_t oldRot_abs = getRotation_abs();

   // - Find model's origin (bottom-left corner) in parent's model space (o)
   //    - This is just m_transl (despite any prior rotations)
   // - Find pivot in parent's model space (p)
   //    - p = (pivot rotated by m_rot) + o
   // - Compute value of o rotated about p by deg degrees
   // - Set m_transl to o
   // - Add deg to m_rot

   // Pivot in parent's model space (or world space if this is root)
   float32_t px = pivot.x * cos(DEG_TO_RAD(m_rot)) - pivot.y * sin(DEG_TO_RAD(m_rot));
   float32_t py = pivot.x * sin(DEG_TO_RAD(m_rot)) + pivot.y * cos(DEG_TO_RAD(m_rot));
   Vec2f p(px + m_transl.x, py + m_transl.y);

   // Rotate bottom-left corner around pivot
   Vec2f o = m_transl - p;
   o.x = o.x * cos(DEG_TO_RAD(deg)) - o.y * sin(DEG_TO_RAD(deg));
   o.y = o.x * sin(DEG_TO_RAD(deg)) + o.y * cos(DEG_TO_RAD(deg));

   m_transl.x = p.x + o.x;
   m_transl.y = p.y + o.y;

   m_rot += deg;

   if (m_shape) m_shape->rotate(deg);
   for (set<pEntity_t>::iterator i = m_children.begin(); i != m_children.end(); ++i)
      (*i)->m_shape->rotate(deg);

   Range bounds = m_boundary;
   recomputeBoundary();

   // Children must dispatch EEntityBoundingBox events too
   for (set<pEntity_t>::iterator i = m_children.begin(); i != m_children.end(); ++i)
      (*i)->parentMovedHandler();

   if (!m_silent) {
      EEvent* event1 = new EEntityBoundingBox(shared_from_this(), bounds, m_boundary);
      EEvent* event2 = new EEntityRotation(shared_from_this(), oldRot, m_rot, oldRot_abs, getRotation_abs());

      onEvent(event1);
      onEvent(event2);

      m_eventManager.queueEvent(event1);
      m_eventManager.queueEvent(event2);
   }
}

//===========================================
// Entity::setShape
//===========================================
void Entity::setShape(std::unique_ptr<Primitive> shape) {
   Range bounds = m_boundary;
   Primitive* oldShape = m_shape ? m_shape->clone() : NULL;

   m_shape = std::move(shape);
   if (m_shape) m_shape->rotate(m_rot);
   if (m_shape) m_shape->scale(m_scale);
   recomputeBoundary();

   if (!m_silent) {
      EEvent* event1 = new EEntityBoundingBox(shared_from_this(), bounds, m_boundary);
      EEvent* event2 = new EEntityShape(shared_from_this(), pPrimitive_t(oldShape), pPrimitive_t(m_shape->clone()));

      onEvent(event1);
      onEvent(event2);

      m_eventManager.queueEvent(event1);
      m_eventManager.queueEvent(event2);
   }
}

//===========================================
// Entity::scale
//===========================================
void Entity::scale(float32_t x, float32_t y) {
   Range bounds = m_boundary;
   Primitive* oldShape = m_shape ? m_shape->clone() : NULL;

   if (m_shape) m_shape->scale(Vec2f(x, y));
   m_scale.x *= x;
   m_scale.y *= y;
   recomputeBoundary();

   if (!m_silent) {
      EEvent* event1 = new EEntityBoundingBox(shared_from_this(), bounds, m_boundary);
      EEvent* event2 = new EEntityShape(shared_from_this(), pPrimitive_t(oldShape), pPrimitive_t(m_shape->clone()));

      onEvent(event1);
      onEvent(event2);

      m_eventManager.queueEvent(event1);
      m_eventManager.queueEvent(event2);
   }
}

//===========================================
// Entity::parentMovedHandler
//===========================================
void Entity::parentMovedHandler() {
   Range bounds = m_boundary;
   recomputeBoundary();

   if (!m_silent)
      m_eventManager.queueEvent(new EEntityBoundingBox(shared_from_this(), bounds, m_boundary));
}

//===========================================
// Entity::~Entity
//===========================================
Entity::~Entity() {}


}
