/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __UI_BUTTON_HPP__
#define __UI_BUTTON_HPP__


#include <boost/shared_ptr.hpp>
#include "../Sprite.hpp"
#include "EntityUi.hpp"
#include "../xml/xml.hpp"
#ifdef DEBUG
#include <ostream>
#endif


namespace Dodge {


class UiButton : public Sprite, public EntityUi {
   public:
      explicit UiButton(const XmlNode data)
         : Asset(internString("UiButton")),
           Entity(data.firstChild().firstChild()),
           Sprite(data.firstChild()),
           EntityUi(this),
           m_state(NO_FOCUS_IDLE),
           m_mouseOver(false),
           m_btn1Pressed(false),
           m_onClick(&UiButton::void_entityPtr),
           m_onRelease(&UiButton::void_entityPtr) {

         XML_NODE_CHECK(data, UiButton);
      }

      UiButton(long type, pTexture_t texture)
         : Asset(internString("UiButton")),
           Entity(type),
           Sprite(type, texture),
           EntityUi(this),
           m_state(NO_FOCUS_IDLE),
           m_mouseOver(false),
           m_btn1Pressed(false),
           m_onClick(&UiButton::void_entityPtr),
           m_onRelease(&UiButton::void_entityPtr) {}

      UiButton(long name, long type, pTexture_t texture)
         : Asset(internString("UiButton")),
           Entity(name, type),
           Sprite(name, type, texture),
           EntityUi(this),
           m_state(NO_FOCUS_IDLE),
           m_mouseOver(false),
           m_btn1Pressed(false),
           m_onClick(&UiButton::void_entityPtr),
           m_onRelease(&UiButton::void_entityPtr) {}

      UiButton(const UiButton& copy)
         : Asset(internString("UiButton")),
           Entity(copy),
           Sprite(copy),
           EntityUi(copy, this),
           m_state(NO_FOCUS_IDLE),
           m_mouseOver(false),
           m_btn1Pressed(false),
           m_onClick(&UiButton::void_entityPtr),
           m_onRelease(&UiButton::void_entityPtr) {}

      UiButton(const UiButton& copy, long name)
         : Asset(internString("UiButton")),
           Entity(copy, name),
           Sprite(copy, name),
           EntityUi(copy, this),
           m_state(NO_FOCUS_IDLE),
           m_mouseOver(false),
           m_btn1Pressed(false),
           m_onClick(&UiButton::void_entityPtr),
           m_onRelease(&UiButton::void_entityPtr) {}

      virtual size_t getSize() const;
      virtual UiButton* clone() const;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual void assignData(const XmlNode data);

      virtual void addToWorld();
      virtual void removeFromWorld();

      virtual void update();

      virtual void onKeyDown(int key);
      virtual void onKeyUp(int key);
      virtual void onBtn1Press(float32_t x, float32_t y);
      virtual void onBtn1Release(float32_t x, float32_t y);
      virtual void onGainFocus();
      virtual void onLoseFocus();
      virtual void onHoverOn(float32_t x, float32_t y);
      virtual void onHoverOff(float32_t x, float32_t y);

      inline void setOnPressHandler(Functor<void, TYPELIST_1(pEntity_t)> func);
      inline void setOnReleaseHandler(Functor<void, TYPELIST_1(pEntity_t)> func);

      // Do nothing function
      static void void_entityPtr(pEntity_t) {}

   private:
      typedef enum {
         NO_FOCUS_IDLE,
         GAIN_FOCUS,
         LOSE_FOCUS,
         FOCUS_IDLE,
         PRESS,
         RELEASE
      } state_t;

      state_t m_state;
      bool m_mouseOver;
      bool m_btn1Pressed;

      Functor<void, TYPELIST_1(pEntity_t)> m_onClick;
      Functor<void, TYPELIST_1(pEntity_t)> m_onRelease;
};

typedef boost::shared_ptr<UiButton> pUiButton_t;

//===========================================
// UiButton::setOnPressHandler
//===========================================
inline void UiButton::setOnPressHandler(Functor<void, TYPELIST_1(pEntity_t)> func) {
   m_onClick = func;
}

//===========================================
// UiButton::setOnReleaseHandler
//===========================================
inline void UiButton::setOnReleaseHandler(Functor<void, TYPELIST_1(pEntity_t)> func) {
   m_onRelease = func;
}


}


#endif
