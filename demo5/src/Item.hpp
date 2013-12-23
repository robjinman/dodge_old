#ifndef __ITEM_HPP__
#define __ITEM_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/Entity.hpp>
#include <dodge/xml/xml.hpp>


class Item : public Dodge::IAuxData {
   public:
      Item(const Dodge::XmlNode data);
      Item(const Item& item);

      virtual size_t getSize() const;
      virtual Item* clone() const;
      virtual void assignData(const Dodge::XmlNode data);

      void setPendingDeletion();

      inline void setSolid(bool b);
      inline bool isSolid() const;

      inline void setEntity(Dodge::Entity* entity);
      inline Dodge::Entity* getEntity() const;

      // TODO: dbg_print

      virtual ~Item();

   private:
      Dodge::Entity* m_entity;
      bool m_solid;
};

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

//===========================================
// Item::setEntity
//===========================================
void Item::setEntity(Dodge::Entity* entity) {
   m_entity = entity;
}

//===========================================
// Item::getEntity
//===========================================
Dodge::Entity* Item::getEntity() const {
   return m_entity;
}


#endif
