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
#include "xml/xml.hpp"


namespace Dodge {


class Entity;

class EntityTransformations {
   public:
      EntityTransformations(Entity* entity, const XmlNode data);

      explicit EntityTransformations(Entity* entity)
         : m_entity(entity) {}

      EntityTransformations(const EntityTransformations& copy, Entity* entity);

      virtual size_t getSize() const;
      virtual void assignData(const XmlNode data);

      inline void addTransformation(pTransformation_t trans);

      void playTransformation(long name);
      void resumeTransformations();
      inline void pauseTransformation(long name);
      void pauseTransformations();
      inline void stopTransformation(long name);
      void stopTransformations();
      void finishTransformation(long name);
      void finishTransformations();

      int numActiveTransformations() const;
      Transformation::state_t getTransState(long name) const;

      virtual void update();
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      virtual ~EntityTransformations() {}

   private:
      void updateTransformation(pTransformation_t t);

      std::map<long, pTransformation_t> m_transformations;
      Entity* m_entity;
};

//===========================================
// EntityTransformations::addTransformation
//===========================================
inline void EntityTransformations::addTransformation(pTransformation_t trans) {
   m_transformations[trans->getName()] = trans;
}

//===========================================
// EntityTransformations::pauseTransformation
//===========================================
inline void EntityTransformations::pauseTransformation(long name) {
   std::map<long, pTransformation_t>::iterator it = m_transformations.find(name);
   if (it != m_transformations.end()) it->second->pause();
}

//===========================================
// EntityTransformations::stopTransformation
//===========================================
inline void EntityTransformations::stopTransformation(long name) {
   std::map<long, pTransformation_t>::iterator it = m_transformations.find(name);
   if (it != m_transformations.end()) it->second->stop();
}


}


#endif /*!__ENTITY_TRANSFORMATIONS_HPP__*/
