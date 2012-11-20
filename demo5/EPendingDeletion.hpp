#ifndef __E_PENDING_DELETION_HPP__
#define __E_PENDING_DELETION_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include "Item.hpp"


class EPendingDeletion : public Dodge::EEvent {
   public:
      EPendingDeletion(pItem_t item_)
         : EEvent(Dodge::internString("pendingDeletion")), item(item_) {}

      pItem_t item;
};


#endif
