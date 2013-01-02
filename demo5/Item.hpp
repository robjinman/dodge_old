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

      inline void setSolid(bool b);
      inline bool isSolid() const;

      // TODO: dbg_print

      virtual ~Item();

   private:
      bool m_solid;
};

typedef boost::shared_ptr<Item> pItem_t;

//===========================================
// Item::setSolid
//===========================================
void Item::setSolid(bool b) {
   m_solid = b;
}

//===========================================
// Item::isSolid
//===========================================
bool Item::isSolid() const {
   return m_solid;
}


#endif
