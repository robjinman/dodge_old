/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__


#include <map>
#include <string>
#include <dodge/dodge.hpp>
#include "Player.hpp"


class Application {
   public:
      Application();

      void onExit(Functor<void, TYPELIST_0()> callBack);
      void begin(int argc, char** argv);

   private:
      void quit();
      void keyDown(int key);
      void keyUp(int key);
      void keyboard();
      void onWindowResize(int w, int h);
      void deletePending(Dodge::EEvent* event);
      void deleteAsset(Dodge::pAsset_t asset);
      Dodge::pAsset_t constructAsset(const Dodge::XmlNode data);
      void setMapSettings(const Dodge::XmlNode data);
      void draw() const;
      void update();
      void updateViewArea();

      void exitDefault();

      void computeFrameRate();

      Functor<void, TYPELIST_0()> m_onExit;

      Dodge::Renderer&                    m_renderer;
      int                                 m_currentMap;
      std::map<int, bool>                 m_keyState;
      std::vector<int>                    m_dirKeyStack;
      Dodge::WinIO                        m_win;
      Dodge::EventManager                 m_eventManager;
      Dodge::Range                        m_viewArea;
      Dodge::float32_t                    m_zoomLevel;
#ifdef DEBUG
      bool                                dbg_worldSpaceVisible;
#endif
      pPlayer_t                           m_player;
      double                              m_frameRate;

      Dodge::AssetManager                 m_assetManager;
      Dodge::WorldSpace                   m_worldSpace;
      std::map<long, Dodge::pEntity_t>    m_entities;

      Dodge::Colour                       m_bgColour;

      Dodge::MapLoader&                   m_mapLoader;
};


#endif
