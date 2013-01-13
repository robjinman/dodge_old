/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __ITEM_HPP__
#define __ITEM_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/Entity.hpp>
#include <dodge/xml/xml.hpp>


class Item : virtual public Dodge::Entity {
   public:
      Item(const Dodge::XmlNode data);
      Item(const Item& item);
      Item(const Item& item, long name);

      virtual size_t getSize() const;
      virtual Item* clone() const;
      virtual void assignData(const Dodge::XmlNode data);

      void setPendingDeletion();

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

      virtual ~Item();

   private:
      // ...
};

typedef boost::shared_ptr<Item> pItem_t;


#endif
