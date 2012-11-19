#ifndef __ITEM_HPP__
#define __ITEM_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/Sprite.hpp>
#include <dodge/xml/xml.hpp>


class Item : public Dodge::Sprite {
   public:
      Item(const Dodge::XmlNode data);
      Item(const Item& item) : Sprite(item) {}
      Item(const Item& item, long name) : Sprite(item, name) {}

      virtual void draw() const;
      virtual void update();
      virtual Item* clone() const;
      virtual void assignData(const Dodge::XmlNode data);

      void setPendingDeletion();

      virtual ~Item() {}
};

typedef boost::shared_ptr<Item> pItem_t;


#endif
