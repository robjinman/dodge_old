/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__


#include <map>
#include <string>
#include <dodge/dodge.hpp>
#include "Player.hpp"
#include "MapLoader.hpp"


class Application {
   public:
      Application()
         : m_onExit(Functor<void, TYPELIST_0()>(this, &Application::exitDefault)),
           m_renderer(Dodge::Renderer::getInstance()),
           m_frameRate(60.0) {}

      void onExit(Functor<void, TYPELIST_0()> callBack);
      void begin(int argc, char** argv);

   private:
      void quit();
      void keyDown(int key);
      void keyUp(int key);
      void keyboard();
      void onWindowResize(int w, int h);
      void deletePending(Dodge::EEvent* event);
      void draw() const;
      void update();
      void updateViewArea();

      void exitDefault();

      void computeFrameRate();

      Functor<void, TYPELIST_0()> m_onExit;

      Dodge::Renderer&           m_renderer;
      int                        m_currentMap;
      std::map<int, bool>        m_keyState;
      std::vector<int>           m_dirKeyStack;
      Dodge::WinIO               m_win;
      Dodge::EventManager        m_eventManager;
      Dodge::Range               m_viewArea;
#ifdef DEBUG
      bool                       dbg_worldSpaceVisible;
#endif
      pPlayer_t                  m_player;
      double                     m_frameRate;

      MapLoader                  m_mapLoader;
      gameMap_t                  m_gameMap;
};


#endif
