#ifndef __PLAYER_HPP__
#define __PLAYER_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/Grid.hpp>
#include <dodge/EEvent.hpp>
//#include <dodge/physics/EntityPhysics.hpp>
#include <dodge/math/CompoundPoly.hpp>
#include <dodge/Platform.hpp>
#include "rapidxml.hpp"
#include "Item.hpp"


class Player : public Item, public Dodge::EntityPhysics {
   public:
      typedef enum { DIG_MODE, PLATFORM_MODE } mode_t;

      Player(Dodge::pGrid_t grid, const Dodge::CompoundPoly& zeroG)
         : Item(),
           EntityPhysics(this, Dodge::physOpts_t(false, true, 1.0, 0.3)),
           m_grid(grid),
           m_zeroGRegion(zeroG),
           m_mode(DIG_MODE) { init(); }

      Player(const Player& copy, Dodge::pGrid_t grid, const Dodge::CompoundPoly& zeroG)
         : Item(copy),
           EntityPhysics(this, Dodge::physOpts_t(false, true, 1.0, 0.3)),
           m_grid(grid),
           m_zeroGRegion(zeroG),
           m_mode(DIG_MODE) { init(); }

      void jump();
      void moveLeft() { move(0); }
      void moveUp() { move(1); }
      void moveRight() { move(2); }
      void moveDown() { move(3); }

      mode_t getMode() const { return m_mode; }

      virtual void addToWorld();
      virtual void removeFromWorld() { EntityPhysics::removeFromWorld(); }

      virtual void assignData(const rapidxml::xml_node<>* data);
      virtual void update();
      virtual void onEvent(const Dodge::pEEvent_t event);

      virtual ~Player() {}

   private:
      void init();
      void move(int dir);
      bool grounded() const;
      void snapToGridV(double offset = 0.0);
      void snapToGridH(double offset = 0.0);

      Dodge::pGrid_t m_grid;
      const Dodge::CompoundPoly& m_zeroGRegion;
      mode_t m_mode;
      Dodge::CompoundPoly m_footSensor;
      Dodge::CompoundPoly m_headSensor;
      Dodge::CompoundPoly m_leftSensor;
      Dodge::CompoundPoly m_rightSensor;
      Dodge::CompoundPoly m_midSensor;

      Dodge::Timer m_jumpTimer;
};

typedef boost::shared_ptr<Player> pPlayer_t;


#endif
