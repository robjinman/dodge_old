#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/WorldSpace.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Box2dPhysics.hpp>
#include <dodge/PhysicalSprite.hpp>
#include <dodge/math/math.hpp>
#include <dodge/Timer.hpp>


class Player : public Dodge::PhysicalSprite<Dodge::Box2dPhysics> {
   public:
      typedef enum { DIG_MODE, PLATFORM_MODE } mode_t;
      typedef enum { LEFT, RIGHT, UP, DOWN } dir_t;

      Player(const Dodge::XmlNode data);
      Player(const Player& copy, long name);

      void jump();
      inline bool moveLeft();
      inline bool moveUp();
      inline bool moveRight();
      inline bool moveDown();

      inline mode_t getMode() const;
      inline dir_t facingDir() const;

      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual size_t getSize() const;
      virtual Player* clone() const;
      virtual void assignData(const Dodge::XmlNode data);
      virtual void update();
      virtual void onEvent(const Dodge::EEvent* event);

      virtual void draw() const;

      virtual ~Player() {}

#ifdef DEBUG
      static const Dodge::byte_t DBG_DRAW_SENSORS = 1 << 0;
      static const Dodge::byte_t DBG_DRAW_SHAPE =   1 << 1;

      Dodge::byte_t dbg_flags;
#endif

   private:
      void init();
      bool move(int dir);
      bool grounded() const;
      void snapToGridV(Dodge::float32_t offset = 0.0);
      void snapToGridH(Dodge::float32_t offset = 0.0);

      Dodge::WorldSpace m_worldSpace;
      mode_t m_mode;
      dir_t m_facing;
      Dodge::Vec2f m_gridSize;
      Dodge::Quad m_footSensor;
      Dodge::Quad m_headSensor;
      Dodge::Quad m_leftSensor;
      Dodge::Quad m_rightSensor;
      Dodge::Quad m_midSensor;

      Dodge::float32_t m_speed;
      Dodge::float32_t m_acc;

      Dodge::Timer m_jumpTimer;

      Dodge::float32_t m_modeLocked;
      Dodge::Timer m_modeLockTimer;
};

typedef boost::shared_ptr<Player> pPlayer_t;

//===========================================
// Player::moveLeft
//===========================================
inline bool Player::moveLeft() {
   return move(0);
}

//===========================================
// Player::moveUp
//===========================================
inline bool Player::moveUp() {
   return move(1);
}

//===========================================
// Player::moveRight
//===========================================
inline bool Player::moveRight() {
   return move(2);
}

//===========================================
// Player::moveDown
//===========================================
inline bool Player::moveDown() {
   return move(3);
}

//===========================================
// Player::getMode
//===========================================
inline Player::mode_t Player::getMode() const {
   return m_mode;
}

//===========================================
// Player::facingDir
//===========================================
inline Player::dir_t Player::facingDir() const {
   return m_facing;
}


#endif
