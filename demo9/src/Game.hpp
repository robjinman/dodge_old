#ifndef __GAME_HPP__
#define __GAME_HPP__


#include <map>
#include <vector>
#include <dodge/dodge.hpp>


class Game {
   public:
      int launch(int argc, char** argv);

   private:
      void onKeyDown(int key);
      void onKeyUp(int key);
      void onRightClick(int x, int y);
      void onLeftClick(int x, int y);
      void onWindowResize(int w, int h);
      void keyboard();
      void quit();
      void computeFrameRate();

      bool m_pendingExit;
      std::map<int, bool> m_keyStates;
      double m_frameRate;
      Dodge::pEntity_t m_ePoly;
      Dodge::pEntity_t m_ePlatform0;
      Dodge::pEntity_t m_ePlatform1;
      Dodge::pEntity_t m_eFloor;
      std::vector<Dodge::pEntity_t> m_entities;
      Dodge::WorldSpace m_worldSpace;
};


#endif
