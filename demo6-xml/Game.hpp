#ifndef __GAME_HPP__
#define __GAME_HPP__


#include <map>
#include <vector>
#include <dodge/dodge.hpp>
#include "Item.hpp"
#include "Player.hpp"


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
      boost::shared_ptr<Dodge::Asset> constructAsset(const rapidxml::xml_node<>* data, long proto = -1);
      void playerSetup();
      void uiSetup();

      Dodge::WinIO                     m_win;
      Dodge::Graphics2d                m_graphics2d;
      Dodge::AssetManager              m_assetManager;
      std::map<int, bool>              m_keys;
      std::vector<int>                 m_dirKeyStack;
      double                           m_frameRate;
      int                              m_currentMap;
      pPlayer_t                        m_player;
      std::vector<pItem_t>             m_items;
      Dodge::WorldSpace                m_worldSpace;
      Dodge::EventManager              m_eventManager;
      Dodge::pFont_t                   m_font1;
};


#endif
