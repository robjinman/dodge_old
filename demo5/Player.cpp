#include <set>
#include <dodge/Exception.hpp>
#include <dodge/ETransFinished.hpp>
#include <dodge/StringId.hpp>
#include <dodge/Math.hpp>
#include "Player.hpp"


using namespace std;
using namespace rapidxml;
using namespace Dodge;


//===========================================
// Player::init
//===========================================
void Player::init() {
   Poly poly;
   poly.verts.push_back(Vec2f(1, -4));
   poly.verts.push_back(Vec2f(15, -4));
   poly.verts.push_back(Vec2f(15, 0));
   poly.verts.push_back(Vec2f(1, 0));
   m_footSensor.addPoly(poly);

   poly.verts.clear();
   poly.verts.push_back(Vec2f(1, 16));
   poly.verts.push_back(Vec2f(15, 16));
   poly.verts.push_back(Vec2f(15, 20));
   poly.verts.push_back(Vec2f(1, 20));
   m_headSensor.addPoly(poly);

   poly.verts.clear();
   poly.verts.push_back(Vec2f(-4, 1));
   poly.verts.push_back(Vec2f(0, 1));
   poly.verts.push_back(Vec2f(0, 15));
   poly.verts.push_back(Vec2f(-4, 15));
   m_leftSensor.addPoly(poly);

   poly.verts.clear();
   poly.verts.push_back(Vec2f(16, 1));
   poly.verts.push_back(Vec2f(20, 1));
   poly.verts.push_back(Vec2f(20, 15));
   poly.verts.push_back(Vec2f(16, 15));
   m_rightSensor.addPoly(poly);

   poly.verts.clear();
   poly.verts.push_back(Vec2f(1, 0));
   poly.verts.push_back(Vec2f(15, 0));
   poly.verts.push_back(Vec2f(15, 4));
   poly.verts.push_back(Vec2f(1, 4));
   m_midSensor.addPoly(poly);
}

//===========================================
// Player::snapToGridH
//===========================================
void Player::snapToGridH(double offset) {
   Vec2f pos = getPosition();
   pos.x = floor((pos.x / m_grid->getCellSize().x) + 0.5) * m_grid->getCellSize().x + offset;

   setPosition(pos.x, pos.y);
}

//===========================================
// Player::snapToGridV
//===========================================
void Player::snapToGridV(double offset) {
   Vec2f pos = getPosition();
   pos.y = floor((pos.y / m_grid->getCellSize().y) + 0.5) * m_grid->getCellSize().y + offset;

   setPosition(pos.x, pos.y);
}

//===========================================
// Player::addToWorld
//===========================================
void Player::addToWorld() {
   EntityPhysics::addToWorld();
}

//===========================================
// Player::update
//===========================================
void Player::update() {
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
   }
}

//===========================================
// Player::onEvent
//===========================================
void Player::onEvent(const pEEvent_t event) {
   static long transFinishedStr = internString("transFinished");

   if (event->getType() == transFinishedStr) {
      if (isStationary() && m_mode == DIG_MODE) {
         snapToGridV();
         snapToGridH();
      }
   }
}

//===========================================
// Player::grounded
//===========================================
bool Player::grounded() const {
   set<pEntity_t> items;
   m_grid->surrounding(getPosition(), m_footSensor, items);
   for (set<pEntity_t>::iterator i = items.begin(); i != items.end(); ++i) {
      if (i->get() == this) continue;

      if (Math::compoundPolyOverlaps(getPosition(), m_footSensor, (*i)->getPosition(), (*i)->getBoundingPoly()))
         return true;
   }

   return false;
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
void Player::move(int dir) {
   static long moveLeftStr = internString("moveLeft");
   static long hitFromRightStr = internString("hitFromRight");
   static long moveRightStr = internString("moveRight");
   static long hitFromLeftStr = internString("hitFromLeft");
   static long moveUpStr = internString("moveUp");
   static long hitFromBottomStr = internString("hitFromBottom");
   static long moveDownStr = internString("moveDown");
   static long hitFromTopStr = internString("hitFromTop");

   if (m_mode == DIG_MODE) {
      double dx = 0, dy = 0;
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
   }
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
