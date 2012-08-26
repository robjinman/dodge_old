#ifndef __E_PENDING_DELETION_HPP__
#define __E_PENDING_DELETION_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/StringId.hpp>
#include <dodge/EEvent.hpp>
#include "Item.hpp"


class EPendingDeletion : public Dodge::EEvent {
   public:
      EPendingDeletion(pItem_t item)
         : EEvent(Dodge::internString("pendingDeletion")), m_item(item) {}

      pItem_t getItem() const { return m_item; }

   private:
      pItem_t m_item;
};

typedef boost::shared_ptr<EPendingDeletion> pEPendingDeletion_t;


#endif
