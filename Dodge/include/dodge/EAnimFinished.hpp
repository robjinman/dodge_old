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
      EAnimFinished(boost::shared_ptr<Entity> ent, const pAnimation_t anim)
         : EEvent(internString("animFinished")), animation(anim), entity(ent) {}

      const pAnimation_t animation;
      boost::shared_ptr<Entity> entity;
};


}


#endif /*!__E_ANIM_FINISHED_HPP__*/
