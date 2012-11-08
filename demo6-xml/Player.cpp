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
void Player::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Player") != 0)
      throw Exception("Error parsing XML for instance of class Player", __FILE__, __LINE__);

   const xml_node<>* node = data->first_node();
   if (node) Item::assignData(node);
}
