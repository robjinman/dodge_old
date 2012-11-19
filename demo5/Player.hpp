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

      Player(const Dodge::XmlNode data);

      Player(const Player& copy, long name)
         : Item(copy, name),
           Box2dPhysics(this, EntityPhysics::options_t(false, true, 1.0, 0.3)),
           m_mode(DIG_MODE) {}

      void jump();
      void moveLeft() { move(0); }
      void moveUp() { move(1); }
      void moveRight() { move(2); }
      void moveDown() { move(3); }

      mode_t getMode() const { return m_mode; }

      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual Player* clone() const;
      virtual void assignData(const Dodge::XmlNode data);
      virtual void update();
      virtual void onEvent(const Dodge::EEvent* event);

      virtual ~Player() {}

   private:
      void init();
      void move(int dir);
      bool grounded() const;
      void snapToGridV(Dodge::float32_t offset = 0.0);
      void snapToGridH(Dodge::float32_t offset = 0.0);

      Dodge::WorldSpace m_worldSpace;
      mode_t m_mode;
      Dodge::Vec2f m_gridSize;
      Dodge::Polygon m_footSensor;
      Dodge::Polygon m_headSensor;
      Dodge::Polygon m_leftSensor;
      Dodge::Polygon m_rightSensor;
      Dodge::Polygon m_midSensor;

      Dodge::Timer m_jumpTimer;
};

typedef boost::shared_ptr<Player> pPlayer_t;


#endif
