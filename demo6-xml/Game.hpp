#ifndef __GAME_HPP__
#define __GAME_HPP__


#include <map>
#include <vector>
#include <dodge/dodge.hpp>


class Game {
   public:
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
      void loadMap();
      void parseDefinitionFile(const std::string& file);
      Dodge::Asset* constructAsset(const rapidxml::xml_node<>* data);
      Dodge::pEntity_t constructEntity(const rapidxml::xml_node<>* data);
      void playerSetup();
      void uiSetup();

      Dodge::WinIO                     m_win;
      Dodge::Graphics2d                m_graphics2d;
      Dodge::SharedAssetManager        m_assetManager;
      std::map<int, bool>              m_keys;
      std::vector<int>                 m_dirKeyStack;
      double                           m_frameRate;
      int                              m_currentMap;
      Dodge::pSprite_t                 m_player;
      std::vector<Dodge::pEntity_t>    m_entities;
      Dodge::WorldSpace                m_worldSpace;
      Dodge::EventManager              m_eventManager;
      Dodge::pFont_t                   m_font1;
};


#endif
