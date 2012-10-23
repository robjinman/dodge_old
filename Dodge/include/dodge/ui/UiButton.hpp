/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __UI_BUTTON_HPP__
#define __UI_BUTTON_HPP__


#include <boost/shared_ptr.hpp>
#include "../Sprite.hpp"
#include "EntityUi.hpp"


namespace Dodge {


class UiButton : public Sprite, public EntityUi {
   public:
      UiButton(long type, pTexture_t texture)
         : Sprite(type, texture), EntityUi(this), m_state(BTN_IDLE), m_mouseOver(false), m_btn1Pressed(false) {}

      UiButton(long name, long type, pTexture_t texture)
         : Sprite(name, type, texture), EntityUi(this), m_state(BTN_IDLE), m_mouseOver(false), m_btn1Pressed(false) {}

      UiButton(const UiButton& copy)
         : Sprite(copy), EntityUi(copy, this), m_state(BTN_IDLE), m_mouseOver(false), m_btn1Pressed(false) {}

      UiButton(const UiButton& copy, long name)
         : Sprite(copy, name), EntityUi(copy, this), m_state(BTN_IDLE), m_mouseOver(false), m_btn1Pressed(false) {}

      virtual void update();

      virtual void onBtn1Press(float32_t x, float32_t y);
      virtual void onBtn1Release(float32_t x, float32_t y);
      virtual void onHoverOn(float32_t x, float32_t y);
      virtual void onHoverOff(float32_t x, float32_t y);

   private:
      typedef enum {
         BTN_IDLE,
         BTN_PRESS,
         BTN_RELEASE,
         BTN_HOVER_ON,
         BTN_HOVER_OFF
      } state_t;

      state_t m_state;
      bool m_mouseOver;
      bool m_btn1Pressed;
};

typedef boost::shared_ptr<UiButton> pUiButton_t;


}


#endif
