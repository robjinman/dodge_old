/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <sstream>
#include <cstdio>
#include <Entity.hpp>
#include <Exception.hpp>
#include <StringId.hpp>


using namespace rapidxml;
using namespace std;


namespace Dodge {


EventManager Entity::m_eventManager = EventManager();
int Entity::m_count = 0;


#ifdef DEBUG
//===========================================
// Entity::dbg_print
//===========================================
void Entity::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; i++) out << "\t";
   out << "Entity\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "name: " << getInternedString(m_name) << "\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "type: " << getInternedString(m_type) << "\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "scale: (" << m_scale.x << ", " << m_scale.y << ")\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "translation: (" << m_transl.x << ", " << m_transl.y << ")\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "z: " << m_z << "\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "rotation: " << m_rot << "\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "shape (src):\n";
   m_srcShape->dbg_print(out, tab + 1);

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "shape (transformed) (up-to-date: " << (m_bTransShape ? "true" : "false") << "):\n";
   m_transShape->dbg_print(out, tab + 1);

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "width (up-to-date: " << (m_bTransShape ? "true" : "false") << "): " << m_width << "\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "width (up-to-date: " << (m_bTransShape ? "true" : "false") << "): " << m_height << "\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "parent: " << (m_parent ? getInternedString(m_parent->getName()) : "NULL") << "\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "children: ";
   if (m_children.size() > 0) {
      out << "\n";
      for (set<pEntity_t>::const_iterator c = m_children.begin(); c != m_children.end(); ++c) {
         for (int i = 0; i < tab + 2; i++) out << "\t";
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
Entity::Entity(long name, long type)
   : m_name(name), m_type(type), m_scale(1.f, 1.f), m_transl(0.f, 0.f),
      m_z(1.f), m_rot(0.f), m_bTransShape(false), m_parent(NULL) {

   ++m_count;
}

//===========================================
// Entity::Entity
//===========================================
Entity::Entity(long type)
   : m_type(type), m_scale(1.f, 1.f), m_transl(0.f, 0.f), m_z(1.f),
      m_rot(0.f), m_bTransShape(false), m_parent(NULL) {

   m_name = generateName();

   ++m_count;
}

//===========================================
// Entity::Entity
//
// Construct deep copy.
//===========================================
Entity::Entity(const Entity& copy) : m_parent(NULL) {
   m_type = copy.m_type;
   m_scale = copy.m_scale;

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
   m_scale = copy.m_scale;
   m_transl = copy.m_transl;
   m_z = copy.m_z;
   m_rot = copy.m_rot;

   m_srcShape = copy.m_srcShape ? unique_ptr<Primitive>(copy.m_srcShape->clone()) : unique_ptr<Primitive>();
   m_transShape = copy.m_transShape ? unique_ptr<Primitive>(copy.m_transShape->clone()) : unique_ptr<Primitive>();
   m_bTransShape = copy.m_bTransShape;

   m_width = copy.m_width;
   m_height = copy.m_height;
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
void Entity::assignData(const rapidxml::xml_node<>* data) {
   if (strcmp(data->name(), "Entity") != 0)
      throw Exception("Error parsing XML for instance of class Entity", __FILE__, __LINE__);

   const xml_attribute<>* attr = data->first_attribute();
   if (attr && strcmp(attr->name(), "type") == 0) {
      m_type = internString(attr->value());
      attr = attr->next_attribute();
   }
   if (attr && strcmp(attr->name(), "name") == 0) {
      m_name = internString(attr->value());
   }
   const xml_node<>* node = data->first_node();
   if (node && strcmp(node->name(), "translation") == 0) {
      const xml_node<>* child = node->first_node();
      if (child) m_transl.assignData(child);
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "z") == 0) {
      sscanf(node->value(), "%f", &m_z);
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "rotation") == 0) {
      sscanf(node->value(), "%f", &m_rot);
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "scale") == 0) {
      const xml_node<>* child = node->first_node();
      if (child) m_scale.assignData(child);
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "shape") == 0) {
      const xml_node<>* child = node->first_node();
      if (child) {
//         m_srcShape = unique_ptr<Primitive>(primitiveFactory.create(child));
         recomputeShape();
      }
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "children") == 0) {
      const xml_node<>* child = node->first_node();
      while (child) {
         // TODO

         child = child->next_sibling();
      }
   }
}

//===========================================
// Entity::getTranslation_abs
//===========================================
Vec3f Entity::getTranslation_abs() const {
   if (m_parent) {
      float32_t x = m_transl.x * cos(DEG_TO_RAD(m_parent->getRotation_abs()))
         - m_transl.y * sin(DEG_TO_RAD(m_parent->getRotation_abs()));

      float32_t y = m_transl.x * sin(DEG_TO_RAD(m_parent->getRotation_abs()))
         + m_transl.y * cos(DEG_TO_RAD(m_parent->getRotation_abs()));

      Vec3f t = m_parent->getTranslation_abs();
      t.z = 0.f;

      return t + Vec3f(x, y, m_z);
   }
   else {
      return getTranslation();
   }
}

//===========================================
// Entity::rotate
//
// Pivot is in model space
//===========================================
void Entity::rotate(float32_t deg, const Vec2f& pivot) {
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
}

//===========================================
// Entity::recomputeShape
//
// Computes the entity's bounding polygon after scaling and rotation
//===========================================
void Entity::recomputeShape() const {
   m_transShape = unique_ptr<Primitive>(m_srcShape->clone());
   m_transShape->rotate(getRotation_abs(), Vec2f(0.f, 0.f));
   m_transShape->scale(m_scale);

   // TODO
//   Vec2f min = m_transShape->computeMinimum();
//   Vec2f max = m_transShape->computeMaximum();

//   m_width = max.x - min.x;
//   m_height = max.y - min.y;

   m_bTransShape = true;
}


}
