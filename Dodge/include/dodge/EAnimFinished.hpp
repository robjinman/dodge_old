/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __E_ANIM_FINISHED_HPP__
#define __E_ANIM_FINISHED_HPP__


#include "StringId.hpp"
#include "EEvent.hpp"
#include <boost/shared_ptr.hpp>


namespace Dodge {


class Animation;
class Entity;

class EAnimFinished : public EEvent {
   public:
      EAnimFinished(boost::shared_ptr<Entity> entity, const Animation* anim)
         : EEvent(internString("animFinished")), m_anim(anim), m_entity(entity) {}

      inline const Animation* getAnimation() const;
      inline boost::shared_ptr<Entity> getEntity() const;

   private:
      const Animation* m_anim;
      boost::shared_ptr<Entity> m_entity;
};

//===========================================
// EAnimFinished::getAnimation
//===========================================
inline const Animation* EAnimFinished::getAnimation() const {
   return m_anim;
}

//===========================================
// EAnimFinished::getEntity
//===========================================
inline boost::shared_ptr<Entity> EAnimFinished::getEntity() const {
   return m_entity;
}


}


#endif /*!__E_ANIM_FINISHED_HPP__*/
