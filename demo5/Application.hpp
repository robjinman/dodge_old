#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__


#include <map>
#include <string>
#include <dodge/dodge.hpp>
#include "Player.hpp"


class Application {
   public:
      Application()
         : m_onExit(Functor<void, TYPELIST_0()>(this, &Application::exitDefault)) {}

      void onExit(Functor<void, TYPELIST_0()> callBack);
      void begin();

#ifdef DEBUG
      static const Dodge::byte_t DBG_DRAW_WORLDSPACE = 1 << 0;

      Dodge::byte_t dbg_flags;
#endif

   private:
      void quit();
      void keyDown(int key);
      void keyUp(int key);
      void keyboard();
      void onWindowResize(int w, int h);
      void deletePending(Dodge::EEvent* event);
      void draw();
      void update();

      void exitDefault();

      boost::shared_ptr<Dodge::Asset> constructAsset(const Dodge::XmlNode data, long proto, bool addToWorld);
      pItem_t constructItem(const Dodge::XmlNode data) const;
      void loadAssets_r(const std::string& file, int depth = 0);
      void loadMapSettings(const Dodge::XmlNode data);
      void loadMap();

      void computeFrameRate();

      Functor<void, TYPELIST_0()> m_onExit;

      Dodge::Graphics2d          m_graphics2d;
      Dodge::Renderer            m_renderer;
      int                        m_currentMap;
      std::map<int, bool>        m_keyState;
      std::vector<int>           m_dirKeyStack;
      Dodge::WinIO               m_win;
      Dodge::EventManager        m_eventManager;
      Dodge::WorldSpace          m_worldSpace;
      Dodge::AssetManager        m_assetManager;
      std::map<long, pItem_t>    m_items;
      pPlayer_t                  m_player;
      Dodge::float32_t           m_frameRate;

      Dodge::Colour m_bgColour;
      Dodge::Vec2f m_mapSize;
      long m_fillerTileId;
};


#endif
