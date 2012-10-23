/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <map>
#include <cstring>
#include <EntityTransformations.hpp>
#include <Entity.hpp>
#include <ETransFinished.hpp>


using namespace rapidxml;
using namespace std;


namespace Dodge {


#ifdef DEBUG
//===========================================
// EntityTransformations::dbg_print
//===========================================
void EntityTransformations::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "EntityTransformations\n";

   for (map<long, pTransformation_t>::const_iterator it = m_transformations.begin(); it != m_transformations.end(); ++it)
      it->second->dbg_print(out, tab + 1);
}
#endif

//===========================================
// EntityTransformations::EntityTransformations
//===========================================
EntityTransformations::EntityTransformations(const EntityTransformations& copy, Entity* entity)
   : m_entity(entity) {

   try {
      map<long, pTransformation_t>::const_iterator it = copy.m_transformations.begin();
      while (it != copy.m_transformations.end()) {
         pTransformation_t trans(new Transformation(*it->second));
         m_transformations[it->first] = trans;
         ++it;
      }
   }
   catch (bad_alloc& e) {
      Exception ex("Error constructing EntityTransformations; ", __FILE__, __LINE__);
      ex.append(e.what());
      throw ex;
   }
}

//===========================================
// EntityTransformations::assignData
//===========================================
void EntityTransformations::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "EntityTransformations") != 0)
      throw Exception("Error parsing XML for instance of class EntityTransformations", __FILE__, __LINE__);

   xml_node<>* node = data->first_node();
   while (node) {
      if (strcmp(node->name(), "Transformation") == 0) {
         pTransformation_t trans(new Transformation());
         trans->assignData(node);
         m_transformations[trans->getName()] = trans;
      }
      node = node->next_sibling();
   }
}

//===========================================
// EntityTransformations::playTransformation
//===========================================
void EntityTransformations::playTransformation(long name) {
   map<long, pTransformation_t>::iterator it = m_transformations.find(name);
   if (it == m_transformations.end()) return;

   it->second->play();
}

//===========================================
// EntityTransformations::update
//===========================================
void EntityTransformations::update() {
   map<long, pTransformation_t>::iterator it = m_transformations.begin();
   while (it != m_transformations.end()) {
      it->second->update();

      const TransFrame* frame = it->second->getCurrentFrame();
      if (frame) {
         m_entity->translate(frame->delta.x, frame->delta.y);
         if (frame->rot != 0.f) m_entity->rotate(frame->rot);
         if (frame->scale != Vec2f(1.f, 1.f)) m_entity->scale(frame->scale);

         if (it->second->getCurrentFrameIndex() == it->second->getNumFrames() * it->second->getSmooth()) {
            try {
               ETransFinished* event = new ETransFinished(m_entity->getSharedPtr(), it->second);
               m_entity->onEvent(event);
            }
            catch (bad_alloc& e) {
               Exception ex("Error updating EntityTransformations; ", __FILE__, __LINE__);
               ex.append(e.what());
               throw ex;
            }
         }
      }

      ++it;
   }
}

//===========================================
// EntityTransformations::numActiveTransformations
//===========================================
int EntityTransformations::numActiveTransformations() const {
   int n = 0;

   map<long, pTransformation_t>::const_iterator it = m_transformations.begin();
   while (it != m_transformations.end()) {
      if (it->second->getState() == Transformation::PLAYING) n++;
      ++it;
   }

   return n;
}

//===========================================
// EntityTransformations::getTransState
//===========================================
Transformation::state_t EntityTransformations::getTransState(long name) const {
   std::map<long, pTransformation_t>::const_iterator it = m_transformations.find(name);

   if (it == m_transformations.end())
      throw Exception("Error retrieving transformation state; Transformation not found", __FILE__, __LINE__);

   return it->second->getState();
}

//===========================================
// EntityTransformations::pauseTransformations
//===========================================
void EntityTransformations::pauseTransformations() {
   for (unsigned int i = 0; i < m_transformations.size(); ++i)
      m_transformations[i]->pause();
}

//===========================================
// EntityTransformations::resumeTransformations
//
// Resume those which are currently paused
//===========================================
void EntityTransformations::resumeTransformations() {
   for (unsigned int i = 0; i < m_transformations.size(); ++i) {
      if (m_transformations[i]->getState() == Transformation::PAUSED)
         m_transformations[i]->play();
   }
}

//===========================================
// EntityTransformations::stopTransformations
//===========================================
void EntityTransformations::stopTransformations() {
   for (unsigned int i = 0; i < m_transformations.size(); ++i)
      m_transformations[i]->stop();
}


}