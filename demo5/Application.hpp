#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__


#include <map>
#include <string>
#include <dodge/dodge.hpp>
#include "Player.hpp"


class Application {
   public:
      Application()
         : m_onExit(Functor<void, TYPELIST_0()>(Application::exitDefault)) {}

      void onExit(Functor<void, TYPELIST_0()> callBack);
      void begin();

   private:
      void quit();
      void keyDown(int key);
      void keyUp(int key);
      void keyboard();
      void deletePending(Dodge::pEEvent_t event);
      void drawAndUpdateItems();

      static void exitDefault();

      void mapSetup();
      pItem_t parseItemFile(const std::string& file);

      void computeFrameRate();

      Functor<void, TYPELIST_0()> m_onExit;

      int                        m_currentMap;
      std::map<int, bool>        m_keyState;
      Dodge::WinIO               m_win;
      Dodge::EventManager        m_eventManager;
      Dodge::pGrid_t             m_grid;
      std::map<long, pItem_t>    m_items;
      pPlayer_t                  m_player;
      Dodge::CompoundPoly        m_zeroGRegion;
      double                     m_frameRate;
};


#endif
