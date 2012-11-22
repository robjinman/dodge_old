#ifndef __ITEM_HPP__
#define __ITEM_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/Sprite.hpp>
#include <dodge/math/Vec2f.hpp>
#include <dodge/EventManager.hpp>
#include <dodge/xml/xml.hpp>


class Item : public Dodge::Sprite {
   public:
      Item(const Dodge::XmlNode data)
         : Entity(data.firstChild().firstChild()), Sprite(data.firstChild()) {}

      Item(const Item& copy)
         : Entity(copy), Sprite(copy) {}

      Item(const Item& copy, long name)
         : Entity(copy, name), Sprite(copy, name) {}

      virtual void draw() const { Dodge::Sprite::draw(); }
      virtual void update() { Sprite::update(); }

      virtual Item* clone() const { return new Item(*this); }
      virtual void assignData(const Dodge::XmlNode data);

      void setPendingDeletion();

      virtual ~Item() {}

   private:
      static Dodge::EventManager eventManager;
};

typedef boost::shared_ptr<Item> pItem_t;


#endif
