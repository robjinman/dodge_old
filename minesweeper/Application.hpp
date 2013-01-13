/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __APPLICATION_HPP__
#define __APPLICATION_HPP__


#include <map>
#include <string>
#include <dodge/dodge.hpp>
#include "Item.hpp"


class Application {
   public:
      Application();

      void onExit(Functor<void, TYPELIST_0()> callBack);
      void launch(int argc, char** argv);

   private:
      typedef enum {
         MOUSE_LEFT_BUTTON    = 0,
         MOUSE_MIDDLE_BUTTON  = 1,
         MOUSE_RIGHT_BUTTON   = 2,

         NUM_MOUSE_BUTTONS = 3
      } mouseBtn_t;

      typedef enum {
         ST_RUNNING,
         ST_START_MENU
      } gameState_t;

      void quit();
      void keyDown(int key);
      void keyUp(int key);
      void mouseLeftClick(int x, int y);
      void mouseRightClick(int x, int y);
      void mouseLeftRelease(int x, int y);
      void mouseRightRelease(int x, int y);
      void mouseMove(int x, int y);
      void keyboard();
      void onWindowResize(int w, int h);
      void deletePending(Dodge::EEvent* event);
      void deleteAsset(Dodge::pAsset_t asset);
      Dodge::pAsset_t constructAsset(const Dodge::XmlNode data);
      void setMapSettings(const Dodge::XmlNode data);
      void draw() const;
      void update();
      void populateMap();

      void exitDefault();

      void computeFrameRate();

      Functor<void, TYPELIST_0()>   m_onExit;

      Dodge::Renderer&              m_renderer;
      std::map<int, bool>           m_keyState;
      bool                          m_mouseState[NUM_MOUSE_BUTTONS];
      Dodge::WinIO                  m_win;
      Dodge::EventManager           m_eventManager;
      Dodge::Range                  m_viewArea;
      gameState_t                   m_gameState;
#ifdef DEBUG
      bool                          dbg_worldSpaceVisible;
#endif
      double                        m_frameRate;

      Dodge::AssetManager           m_assetManager;
      Dodge::WorldSpace             m_worldSpace;
      std::map<long, pItem_t>       m_items;

      Dodge::Colour                 m_bgColour;

      Dodge::MapLoader              m_mapLoader;
};


#endif
