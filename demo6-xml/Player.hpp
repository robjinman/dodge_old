#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/xml/xml.hpp>
#include "Item.hpp"


class Player : public Item {
   public:
      Player(const Dodge::XmlNode data)
         : Entity(data.firstChild().firstChild().firstChild()),
           Item(data.firstChild()) {}

      Player(const Player& copy)
         : Entity(copy), Item(copy) {}

      virtual Player* clone() const;
      virtual void assignData(const Dodge::XmlNode data) ;
};

typedef boost::shared_ptr<Player> pPlayer_t;


#endif
