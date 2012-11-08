#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/rapidxml/rapidxml.hpp>
#include "Item.hpp"


class Player : public Item {
   public:
      Player(const rapidxml::xml_node<>* data) : Item(Dodge::nthChild(data, 0)) {}
      Player(const Player& copy) : Item(copy) {}

      virtual Player* clone() const;
      virtual void assignData(const rapidxml::xml_node<>* data) ;
};

typedef boost::shared_ptr<Player> pPlayer_t;


#endif
