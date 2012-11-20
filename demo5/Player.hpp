#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/WorldSpace.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Box2dPhysics.hpp>
#include <dodge/math/math.hpp>
#include "Item.hpp"


class Player : public Item, public Dodge::Box2dPhysics {
   public:
      typedef enum { DIG_MODE, PLATFORM_MODE } mode_t;
      typedef enum { LEFT, RIGHT, UP, DOWN } dir_t;

      Player(const Dodge::XmlNode data);
      Player(const Player& copy, long name);

      void jump();
      bool moveLeft();
      bool moveUp();
      bool moveRight();
      bool moveDown();

      mode_t getMode() const;
      dir_t facingDir() const;

      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual Player* clone() const;
      virtual void assignData(const Dodge::XmlNode data);
      virtual void update();
      virtual void draw() const;
      virtual void onEvent(const Dodge::EEvent* event);

      virtual ~Player() {}

   private:
      inline void init();
      inline bool move(int dir);
      inline bool grounded() const;
      inline void snapToGridV(Dodge::float32_t offset = 0.0);
      inline void snapToGridH(Dodge::float32_t offset = 0.0);

      Dodge::WorldSpace m_worldSpace;
      mode_t m_mode;
      dir_t m_facing;
      Dodge::Vec2f m_gridSize;
      Dodge::Quad m_footSensor;
      Dodge::Quad m_headSensor;
      Dodge::Quad m_leftSensor;
      Dodge::Quad m_rightSensor;
      Dodge::Quad m_midSensor;

      Dodge::Timer m_jumpTimer;
};

typedef boost::shared_ptr<Player> pPlayer_t;

//===========================================
// Player::moveLeft
//===========================================
inline bool moveLeft() {
   return move(0);
}

//===========================================
// Player::moveUp
//===========================================
inline bool moveUp() {
   return move(1);
}

//===========================================
// Player::moveRight
//===========================================
inline bool moveRight() {
   return move(2);
}

//===========================================
// Player::moveDown
//===========================================
inline bool moveDown() {
   return move(3);
}

//===========================================
// Player::getMode
//===========================================
inline mode_t getMode() const {
   return m_mode;
}

//===========================================
// Player::facingDir
//===========================================
inline dir_t facingDir() const {
   return m_facing;
}


#endif
