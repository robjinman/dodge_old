/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ENTITY_TRANSFORMATIONS_HPP__
#define __ENTITY_TRANSFORMATIONS_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include <map>
#include "Transformation.hpp"
#include "rapidxml/rapidxml.hpp"


namespace Dodge {


class Entity;

class EntityTransformations {
   public:
      EntityTransformations(Entity* entity)
         : m_entity(entity) {}

      EntityTransformations(const EntityTransformations& copy, Entity* entity);

      inline void addTransformation(Transformation* trans); // Should be unique_ptr ?

      void playTransformation(long name);
      void resumeTransformations();
      inline void pauseTransformation(long name);
      void pauseTransformations();
      inline void stopTransformation(long name);
      void stopTransformations();

      int numActiveTransformations() const;
      Transformation::state_t getTransState(long name) const;

      virtual void update();
      virtual void assignData(const rapidxml::xml_node<>* data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      virtual ~EntityTransformations() {}

   private:
      std::map<long, Transformation*> m_transformations;
      Entity* m_entity;
};

//===========================================
// EntityTransformations::addTransformation
//===========================================
inline void EntityTransformations::addTransformation(Transformation* trans) {
   m_transformations[trans->getName()] = trans;
}

//===========================================
// EntityTransformations::pauseTransformation
//===========================================
inline void EntityTransformations::pauseTransformation(long name) {
   std::map<long, Transformation*>::iterator it = m_transformations.find(name);
   if (it != m_transformations.end()) it->second->pause();
}

//===========================================
// EntityTransformations::stopTransformation
//===========================================
inline void EntityTransformations::stopTransformation(long name) {
   std::map<long, Transformation*>::iterator it = m_transformations.find(name);
   if (it != m_transformations.end()) it->second->stop();
}


}


#endif /*!__ENTITY_TRANSFORMATIONS_HPP__*/
