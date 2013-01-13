/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ENTITY_UI_HPP__
#define __ENTITY_UI_HPP__


#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include "../../utils/Functor.hpp"
#include "../WinIO.hpp"
#include "../EventManager.hpp"
#include "uiEventTypes.hpp"
#include "../Range.hpp"


namespace Dodge {


class Entity;

class EntityUi {
   public:
      typedef boost::variant <
         Functor<void, TYPELIST_1(boost::shared_ptr<Entity>)>,
         Functor<void, TYPELIST_2(boost::shared_ptr<Entity>, int)>,
         Functor<void, TYPELIST_3(boost::shared_ptr<Entity>, float32_t, float32_t)>
      > callback_t;

      explicit EntityUi(Entity* entity);
      EntityUi(const EntityUi& copy, Entity* entity);

      bool hasFocus() const;
      void registerCallback(uiEvent_t event, callback_t func);

      virtual size_t getSize() const;
      virtual void update() = 0;

      virtual void onBtn1Press(float32_t x, float32_t y) {}
      virtual void onBtn1Release(float32_t x, float32_t y) {}
      virtual void onBtn3Press(float32_t x, float32_t y) {}
      virtual void onBtn3Release(float32_t x, float32_t y) {}
      virtual void onKeyDown(int code) {}
      virtual void onKeyUp(int code) {}
      virtual void onHoverOn(float32_t x, float32_t y) {}
      virtual void onHoverOff(float32_t x, float32_t y) {}

      virtual ~EntityUi();

   private:
      void init();

      void btn1PressHandler(int x, int y);
      void btn1ReleaseHandler(int x, int y);
      void btn3PressHandler(int x, int y);
      void btn3ReleaseHandler(int x, int y);
      void keyDownHandler(int key);
      void keyUpHandler(int key);
      void mouseMoveHandler(int x, int y);
      bool inRange(int x, int y, float32_t& wx, float32_t& wy) const;

      Entity* m_entity;
      bool m_hasFocus;
      bool m_mouseOver;

      typedef std::map<uiEvent_t, callback_t> callbackMap_t;
      callbackMap_t m_callbacks;

      static WinIO m_winIO;
      static EventManager m_eventManager;
};

//===========================================
// EntityUi::registerCallback
//===========================================
inline void EntityUi::registerCallback(uiEvent_t event, callback_t func) {
   m_callbacks.insert(callbackMap_t::value_type(event, func));
}

//===========================================
// EntityUi::hasFocus
//===========================================
inline bool EntityUi::hasFocus() const {
   return m_hasFocus;
}


}


#endif
