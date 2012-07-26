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

   // TODO

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "bounding poly (src):\n";
   m_srcPoly.dbg_print(out, tab + 1);

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "bounding poly (transformed):\n";
   m_transPoly.dbg_print(out, tab + 1);
}
#endif


//===========================================
// Entity::Entity
//===========================================
Entity::Entity(long name, long type)
   : m_name(name), m_type(type), m_scale(1.f, 1.f, 1.f) {

   ++m_count;
}

//===========================================
// Entity::Entity
//===========================================
Entity::Entity(long type)
   : m_type(type), m_scale(1.f, 1.f, 1.f) {

   m_name = generateName();

   ++m_count;
}

//===========================================
// Entity::Entity
//
// Construct deep copy.
//===========================================
Entity::Entity(const Entity& copy) {
   m_name = generateName();
   m_type = copy.m_type;
   m_scale = copy.m_scale;

   deepCopy(copy);

   ++m_count;
}

//===========================================
// Entity::Entity
//
// Construct deep copy.
//===========================================
Entity::Entity(const Entity& copy, long name) {
   deepCopy(copy);
   m_name = name;

   ++m_count;
}

//===========================================
// Entity::deepCopy
//===========================================
void Entity::deepCopy(const Entity& copy) {
   m_name = copy.m_name;
   m_type = copy.m_type;
   m_scale = copy.m_scale;

   m_srcPoly = copy.m_srcPoly;
   m_transPoly = copy.m_transPoly;

   // TODO
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

   // TODO

   const xml_node<>* node = data->first_node();
   if (node && strcmp(node->name(), "boundingPoly") == 0) {
      const xml_node<>* child = node->first_node();
      if (child) m_srcPoly.assignData(child);
      recomputePoly();
   }
}

//===========================================
// Entity::recomputePoly
//===========================================
void Entity::recomputePoly() {
   // TODO
}


}
