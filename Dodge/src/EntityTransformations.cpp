/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <map>
#include <cstring>
#include <EntityTransformations.hpp>
#include <Entity.hpp>
#include <AssetManager.hpp>


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
// EntityTransformations::EntityTransformations
//===========================================
EntityTransformations::EntityTransformations(Entity* entity, const XmlNode data)
   : m_entity(entity) {

   AssetManager assetManager;

   try {
      XML_NODE_CHECK(data, EntityTransformations);

      XmlNode node = data.firstChild();
      while (!node.isNull() && node.name() == "transformation") {
         XmlAttribute attr = node.firstAttribute();
         if (!attr.isNull() && attr.name() == "proto") {
            long id = attr.getLong();

            pTransformation_t trans(dynamic_cast<Transformation*>(assetManager.cloneAsset(id)));

            if (!trans)
               throw XmlException("Bad entity asset id", __FILE__, __LINE__);

            m_transformations[trans->getName()] = trans;
         }
         else {
            pTransformation_t trans(new Transformation(node.firstChild()));
            m_transformations[trans->getName()] = trans;
         }

         node = node.nextSibling();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class EntityTransformations; ");
      throw;
   }
}

//===========================================
// EntityTransformations::getSize
//===========================================
size_t EntityTransformations::getSize() const {

   size_t transSz = 0;
   for (auto i = m_transformations.begin(); i != m_transformations.end(); ++i)
      transSz += sizeof(pair<long, pTransformation_t>) + i->second->getSize();

   return sizeof(EntityTransformations)
      + transSz;
}

//===========================================
// EntityTransformations::assignData
//===========================================
void EntityTransformations::assignData(const XmlNode data) {
   if (data.isNull() || data.name() != "EntityTransformations") return;

   AssetManager assetManager;

   try {
      XmlNode node = data.firstChild();
      while (!node.isNull() && node.name() == "transformation") {
         XmlAttribute attr = node.firstAttribute();
         if (!attr.isNull() && attr.name() == "proto") {
            long id = attr.getLong();

            pTransformation_t trans(dynamic_cast<Transformation*>(assetManager.cloneAsset(id)));
            m_transformations[trans->getName()] = trans;
         }
         else {
            pTransformation_t trans(new Transformation(node.firstChild()));

            if (!trans)
               throw XmlException("Bad entity asset id", __FILE__, __LINE__);

            m_transformations[trans->getName()] = trans;
         }

         node = node.nextSibling();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class EntityTransformations; ");
      throw;
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
// EntityTransformations::finishTransformation
//===========================================
void EntityTransformations::finishTransformation(long name) {
   map<long, pTransformation_t>::iterator it = m_transformations.find(name);
   if (it == m_transformations.end()) return;

   while (it->second->getState() == Transformation::PLAYING)
      updateTransformation(it->second);
}

//===========================================
// EntityTransformations::finishTransformations
//===========================================
void EntityTransformations::finishTransformations() {
   for (auto it = m_transformations.begin(); it != m_transformations.end(); ++it)
      finishTransformation(it->second->getName());
}

//===========================================
// EntityTransformations::updateTransformation
//===========================================
void EntityTransformations::updateTransformation(pTransformation_t t) {
   EventManager eventManager;

   uint_t part = t->getCurrentPartIdx();
   const Transformation::delta_t* delta = t->update();

   if (part != t->getCurrentPartIdx() && !m_entity->isSilent()) {
      ETransPartFinished* event = new ETransPartFinished(m_entity->getSharedPtr(), t);
      m_entity->onEvent(event);
      eventManager.queueEvent(event);
   }

   if (delta) {
      m_entity->translate(delta->transl.x, delta->transl.y);

      if (delta->rot != 0.f) m_entity->rotate(delta->rot, delta->pivot);
      if (delta->scale != Vec2f(1.f, 1.f)) m_entity->scale(delta->scale);

      if (t->getCurrentFrameNumber() == t->getNumFrames() && !m_entity->isSilent()) {
         ETransFinished* event = new ETransFinished(m_entity->getSharedPtr(), t);
         m_entity->onEvent(event);
         eventManager.queueEvent(event);
      }
   }
}

//===========================================
// EntityTransformations::update
//===========================================
void EntityTransformations::update() {
   try {
      for (auto it = m_transformations.begin(); it != m_transformations.end(); ++it) {
         updateTransformation(it->second);
      }
   }
   catch (bad_alloc& e) {
      Exception ex("Error updating EntityTransformations; ", __FILE__, __LINE__);
      ex.append(e.what());
      throw ex;
   }
}

//===========================================
// EntityTransformations::numActiveTransformations
//===========================================
int EntityTransformations::numActiveTransformations() const {
   int n = 0;

   map<long, pTransformation_t>::const_iterator it = m_transformations.begin();
   while (it != m_transformations.end()) {
      if (it->second->getState() == Transformation::PLAYING) ++n;
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
   for (map<long, pTransformation_t>::iterator it = m_transformations.begin(); it != m_transformations.end(); ++it)
      it->second->pause();
}

//===========================================
// EntityTransformations::resumeTransformations
//
// Resume those which are currently paused
//===========================================
void EntityTransformations::resumeTransformations() {
   for (map<long, pTransformation_t>::iterator it = m_transformations.begin(); it != m_transformations.end(); ++it) {
      if (it->second->getState() == Transformation::PAUSED)
         it->second->play();
   }
}

//===========================================
// EntityTransformations::stopTransformations
//===========================================
void EntityTransformations::stopTransformations() {
   for (map<long, pTransformation_t>::iterator it = m_transformations.begin(); it != m_transformations.end(); ++it)
      it->second->stop();
}


}
