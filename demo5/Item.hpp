#ifndef __ITEM_HPP__
#define __ITEM_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/Entity.hpp>
#include <dodge/xml/xml.hpp>


class Item : virtual public Dodge::Entity {
   public:
      Item(const Dodge::XmlNode data);
      Item(const Item& item) : Entity(item) {}
      Item(const Item& item, long name) : Entity(item, name) {}

      virtual Item* clone() const;
      virtual void assignData(const Dodge::XmlNode data);

      void setPendingDeletion();

      virtual ~Item() {}
};

typedef boost::shared_ptr<Item> pItem_t;


#endif
