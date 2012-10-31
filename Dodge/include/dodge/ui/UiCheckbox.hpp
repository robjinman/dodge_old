/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __UI_CHECK_BOX_HPP__
#define __UI_CHECK_BOX_HPP__


#include <boost/shared_ptr.hpp>
#include "../utils/Functor.hpp"
#include "../Sprite.hpp"
#include "EntityUi.hpp"


namespace Dodge {


class UiCheckbox : public Sprite, public EntityUi {
   public:
      UiCheckbox(long type, pTexture_t texture)
         : Sprite(type, texture),
           EntityUi(this),
           m_state(BTN_IDLE),
           m_mouseOver(false),
           m_btn1Pressed(false),
           m_onClick(&UiCheckbox::void_entityPtrBool),
           m_onRelease(&UiCheckbox::void_entityPtrBool) {}

      UiCheckbox(long name, long type, pTexture_t texture)
         : Sprite(name, type, texture),
           EntityUi(this),
           m_state(BTN_IDLE),
           m_mouseOver(false),
           m_btn1Pressed(false),
           m_onClick(&UiCheckbox::void_entityPtrBool),
           m_onRelease(&UiCheckbox::void_entityPtrBool) {}

      UiCheckbox(const UiCheckbox& copy)
         : Sprite(copy),
           EntityUi(copy, this),
           m_state(BTN_IDLE),
           m_mouseOver(false),
           m_btn1Pressed(false),
           m_onClick(&UiCheckbox::void_entityPtrBool),
           m_onRelease(&UiCheckbox::void_entityPtrBool) {}

      UiCheckbox(const UiCheckbox& copy, long name)
         : Sprite(copy, name),
           EntityUi(copy, this),
           m_state(BTN_IDLE),
           m_mouseOver(false),
           m_btn1Pressed(false),
           m_onClick(&UiCheckbox::void_entityPtrBool),
           m_onRelease(&UiCheckbox::void_entityPtrBool) {}

      virtual void update();

      virtual void onBtn1Press(float32_t x, float32_t y);
      virtual void onBtn1Release(float32_t x, float32_t y);
      virtual void onHoverOn(float32_t x, float32_t y);
      virtual void onHoverOff(float32_t x, float32_t y);

      inline void setOnToggleHandler(Functor<void, TYPELIST_2(pEntity_t, bool)> func);

      inline bool isChecked() const;

      // Do nothing function
      static void void_entityPtrBool(pEntity_t, bool) {}

   private:
      typedef enum {
         CHK_IDLE,
         CHK_PRESS,
         CHK_RELEASE,
         CHK_HOVER_ON,
         CHK_HOVER_OFF
      } state_t;

      state_t m_state;
      bool m_mouseOver;
      bool m_btn1Pressed;
      bool m_checked;

      Functor<void, TYPELIST_2(pEntity_t, bool)> m_onToggle;
};

typedef boost::shared_ptr<UiCheckbox> pUiCheckbox_t;

//===========================================
// UiCheckbox::setOnToggleHandler
//===========================================
inline void UiCheckbox::setOnToggleHandler(Functor<void, TYPELIST_2(pEntity_t, bool)> func) {
   m_onToggle = func;
}

//===========================================
// UiCheckbox::isChecked
//===========================================
inline bool UiCheckbox::isChecked() const {
   return m_checked;
}


}


#endif
