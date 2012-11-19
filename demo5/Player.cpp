#include <set>
#include <dodge/ETransFinished.hpp>
#include <dodge/StringId.hpp>
#include "Player.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Player::Player
//===========================================
Player::Player(const XmlNode data)
   : Item(data.firstChild()), Box2dPhysics(this, data) {

   try {
      XML_NODE_CHECK(data, Player)
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Player; ");
      throw;
   }
}

//===========================================
// Player::clone
//===========================================
Player* Player::clone() const {
   return new Player(*this);
}

//===========================================
// Player::snapToGridH
//===========================================
void Player::snapToGridH(float32_t offset) {
   Vec2f pos = getTranslation_abs();
   pos.x = floor((pos.x / m_gridSize.x) + 0.5) * m_gridSize.x + offset;

   translate(pos - getTranslation_abs());
}

//===========================================
// Player::snapToGridV
//===========================================
void Player::snapToGridV(float32_t offset) {
   Vec2f pos = getTranslation_abs();
   pos.y = floor((pos.y / m_gridSize.y) + 0.5) * m_gridSize.y + offset;

   translate(pos - getTranslation_abs());
}

//===========================================
// Player::addToWorld
//===========================================
void Player::addToWorld() {
   Box2dPhysics::addToWorld();
}

//===========================================
// Player::removeFromWorld
//===========================================
void Player::removeFromWorld() {
   Box2dPhysics::removeFromWorld();
}

//===========================================
// Player::update
//===========================================
void Player::update() {/*
   Item::update();

   if (!(Math::compoundPolyOverlaps(getPosition(), m_footSensor, Vec2f(0, 0), m_zeroGRegion)
      && Math::compoundPolyOverlaps(getPosition(), m_midSensor, Vec2f(0, 0), m_zeroGRegion))) {

      if (m_mode == DIG_MODE) {
         makeDynamic();
      }

      m_mode = PLATFORM_MODE;
   }
   else {
      if (m_mode == PLATFORM_MODE) {
         makeStatic();
      }

      m_mode = DIG_MODE;
   }*/
}

//===========================================
// Player::onEvent
//===========================================
void Player::onEvent(const EEvent* event) {
   static long transFinishedStr = internString("transFinished");

   if (event->getType() == transFinishedStr) {
      if (/*isStationary() && */m_mode == DIG_MODE) {
         snapToGridV();
         snapToGridH();
      }
   }
}

//===========================================
// Player::grounded
//===========================================
bool Player::grounded() const {/*
   set<pEntity_t> items;
   m_grid->surrounding(getPosition(), m_footSensor, items);
   for (set<pEntity_t>::iterator i = items.begin(); i != items.end(); ++i) {
      if (i->get() == this) continue;

      if (Math::compoundPolyOverlaps(getPosition(), m_footSensor, (*i)->getPosition(), (*i)->getBoundingPoly()))
         return true;
   }

   return false;*/
}

//===========================================
// Player::jump
//===========================================
void Player::jump() {
   static long jumpStr = internString("jump");

   if (grounded() && m_jumpTimer.getTime() > 0.2) {
      stopAnimation();
      playAnimation(jumpStr);
      applyLinearImpulse(Vec2f(0.0, 0.06), Vec2f(0.0, 0.0));
      m_jumpTimer.reset();
   }
}

//===========================================
// Player::move
//
// 0 = left, 1 = up, 2 = right, 3 = down
//===========================================
void Player::move(int dir) {/*
   static long moveLeftStr = internString("moveLeft");
   static long hitFromRightStr = internString("hitFromRight");
   static long moveRightStr = internString("moveRight");
   static long hitFromLeftStr = internString("hitFromLeft");
   static long moveUpStr = internString("moveUp");
   static long hitFromBottomStr = internString("hitFromBottom");
   static long moveDownStr = internString("moveDown");
   static long hitFromTopStr = internString("hitFromTop");

   if (m_mode == DIG_MODE) {
      float32_t dx = 0, dy = 0;
      long plyrAnim = 0, eventType = 0;
      switch (dir) {
         case 0:           // Left
            dx = -16.0;
            dy = 0.0;
            plyrAnim = moveLeftStr;
            eventType = hitFromRightStr;
            break;
         case 1:           // Up
            dx = 0.0;
            dy = 16.0;
            plyrAnim = moveUpStr;
            eventType = hitFromBottomStr;
            break;
         case 2:           // Right
            dx = 16.0;
            dy = 0.0;
            plyrAnim = moveRightStr;
            eventType = hitFromLeftStr;
            break;
         case 3:           // Down
            dx = 0.0;
            dy = -16.0;
            plyrAnim = moveDownStr;
            eventType = hitFromTopStr;
            break;
      }

      playAnimation(plyrAnim);

      if (isStationary()) {
         applyTransformation(plyrAnim);

         const set<pEntity_t>& vec = m_grid->atWorldCoords(getPosition() + Vec2f(dx, dy));
         for (set<pEntity_t>::iterator i = vec.begin(); i != vec.end(); ++i) {
            if (i->get() == this) continue;

            pEEvent_t event(new EEvent(eventType));
            (*i)->onEvent(event);
         }
      }
   }
   else if (m_mode == PLATFORM_MODE) {
      switch (dir) {
         case 0:
            if (Math::compoundPolyOverlaps(getPosition(), m_leftSensor, Vec2f(0, 0), m_zeroGRegion)) {
               m_mode = DIG_MODE;
               snapToGridV();
               makeStatic();
               snapToGridH();
               move(0);
            }
            else {
               playAnimation(moveLeftStr);
               if (getLinearVelocity().x > -1.0)
                  applyForce(Vec2f(-0.2, 0.0), Vec2f(0.0, 0.0));
            }
            break;
         case 1:
            jump();
            break;
         case 2:
            if (Math::compoundPolyOverlaps(getPosition(), m_rightSensor, Vec2f(0, 0), m_zeroGRegion)) {
               m_mode = DIG_MODE;
               snapToGridV();
               makeStatic();
               snapToGridH();
               move(2);
            }
            else {
               playAnimation(moveRightStr);
               if (getLinearVelocity().x < 1.0)
                  applyForce(Vec2f(0.2, 0.0), Vec2f(0.0, 0.0));
            }
            break;
         case 3:
            if (Math::compoundPolyOverlaps(getPosition(), m_footSensor, Vec2f(0, 0), m_zeroGRegion)) {
               m_mode = DIG_MODE;
               snapToGridH();
               makeStatic();
               snapToGridV();
               move(3);
            }
            break;
      }
   }*/
}

//===========================================
// Player::assignData
//===========================================
void Player::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Player)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Item") {
         Item::assignData(node);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "Box2dPhysics") {
         Item::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Player; ");
      throw;
   }
}
