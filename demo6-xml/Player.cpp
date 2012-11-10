#include <dodge/Exception.hpp>
#include "Player.hpp"


using namespace Dodge;
using namespace rapidxml;


//===========================================
// Player::clone
//===========================================
Player* Player::clone() const {
   return new Player(*this);
}

//===========================================
// Player::assignData
//===========================================
void Player::assignData(const XmlNode data) {
   if (data.isNull() || data.name() != "Player")
      throw Exception("Error parsing XML for instance of class Player; Expected 'Player' tag", __FILE__, __LINE__);

   Item::assignData(data.firstChild());
}
