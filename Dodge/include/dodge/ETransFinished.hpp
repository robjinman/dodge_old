#ifndef __E_TRANS_FINISHED_HPP__
#define __E_TRANS_FINISHED_HPP__


#include <boost/shared_ptr.hpp>
#include "StringId.hpp"
#include "EEvent.hpp"


namespace Dodge {


class Entity;
class Transformation;

class ETransFinished : public EEvent {
   public:
      ETransFinished(boost::shared_ptr<Entity> entity, const Transformation* trans)
         : EEvent(internString("transFinished")), m_trans(trans), m_entity(entity) {}

      const Transformation* m_trans;
      boost::shared_ptr<Entity> m_entity;
};


}


#endif
