#ifndef __ITEM_HPP__
#define __ITEM_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/Sprite.hpp>
#include <dodge/Vec2f.hpp>
#include <dodge/EventManager.hpp>
#include <dodge/rapidxml/rapidxml.hpp>


class Item : public Dodge::Sprite {
   public:
      Item()
         : Sprite() {}

      Item(const Item& item)
         : Sprite(item) {}

      virtual void draw(const Dodge::Vec2f& at) const { Dodge::Sprite::draw(at); }
      virtual void update() { Sprite::update(); }

      virtual void assignData(const rapidxml::xml_node<>* data);

      void setPendingDeletion();

      virtual ~Item() {}

   private:
      static Dodge::EventManager eventManager;
};

typedef boost::shared_ptr<Item> pItem_t;


#endif
