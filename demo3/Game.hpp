#ifndef __GAME_HPP__
#define __GAME_HPP__


#include <map>
#include <vector>
#include <dodge/dodge.hpp>


class Game {
   public:
      Game()
         : m_renderer(Dodge::Renderer::getInstance()) {}

      void launch();

   private:
      void quit();
      void keyUp(int code);
      void keyDown(int code);
      void keyboard();
      void computeFrameRate();

      void buttonPressHandler(Dodge::pEntity_t entity);
      void buttonReleaseHandler(Dodge::pEntity_t entity);

      void init();
      void playerSetup();
      void uiSetup();

      Dodge::WinIO                     m_win;
      Dodge::Renderer&                 m_renderer;
      std::map<int, bool>              m_keys;
      std::vector<int>                 m_dirKeyStack;
      double                           m_frameRate;
      Dodge::pSprite_t                 m_player;
      std::vector<Dodge::pEntity_t>    m_entities;
      Dodge::WorldSpace                m_worldSpace;
      Dodge::EventManager              m_eventManager;
      Dodge::pFont_t                   m_font1;
};


#endif
