#ifndef __E_PENDING_DELETION_HPP__
#define __E_PENDING_DELETION_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/Entity.hpp>


class EPendingDeletion : public Dodge::EEvent {
   public:
      EPendingDeletion(Dodge::pEntity_t ent)
         : EEvent(Dodge::internString("pendingDeletion")), entity(ent) {}

      Dodge::pEntity_t entity;
};


#endif
