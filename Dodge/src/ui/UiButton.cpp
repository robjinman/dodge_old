/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <ui/UiButton.hpp>
#include <StringId.hpp>


namespace Dodge {


//===========================================
// UiButton::update
//===========================================
void UiButton::update() {
   static long idleStr = internString("idle");
   static long hoverOffStr = internString("hoverOff");
   static long hoverOnStr = internString("hoverOn");
   static long releaseStr = internString("release");

   Sprite::update();

   if (getAnimState() == Animation::STOPPED) {
      switch (m_state) {
         case BTN_IDLE:
            playAnimation(idleStr);
         break;
         case BTN_HOVER_OFF:
            if (m_mouseOver) {
               playAnimation(hoverOnStr);
               m_state = BTN_HOVER_ON;
            }
         break;
         case BTN_HOVER_ON:
            if (!m_mouseOver) {
               playAnimation(hoverOffStr);
               m_state = BTN_HOVER_OFF;
            }
         break;
         case BTN_RELEASE:
            if (m_mouseOver)
               m_state = BTN_HOVER_ON;
            else {
               playAnimation(hoverOffStr);
               m_state = BTN_HOVER_OFF;
            }
         break;
         case BTN_PRESS:
            if (!m_btn1Pressed || !m_mouseOver) {
               playAnimation(releaseStr);
               m_state = BTN_RELEASE;
            }
         break;
      }
   }
}

//===========================================
// UiButton::onBtn1Press
//===========================================
void UiButton::onBtn1Press(float32_t x, float32_t y) {
   static long pressStr = internString("press");

   m_btn1Pressed = true;

   switch (m_state) {
      case BTN_PRESS:
      case BTN_RELEASE:
      break;
      case BTN_HOVER_ON:
      case BTN_HOVER_OFF:
      case BTN_IDLE:
         stopAnimation();
         playAnimation(pressStr);
         m_state = BTN_PRESS;
      break;
   }
}

//===========================================
// UiButton::onBtn1Release
//===========================================
void UiButton::onBtn1Release(float32_t x, float32_t y) {
   static long releaseStr = internString("release");

   m_btn1Pressed = false;

   switch (m_state) {
      case BTN_IDLE:
      case BTN_RELEASE:
      case BTN_HOVER_ON:
      case BTN_HOVER_OFF:
      break;
      case BTN_PRESS:
         if (playAnimation(releaseStr))
            m_state = BTN_RELEASE;
      break;
   }
}

//===========================================
// UiButton::onHoverOn
//===========================================
void UiButton::onHoverOn(float32_t x, float32_t y) {
   static long hoverOnStr = internString("hoverOn");

   m_mouseOver = true;

   switch (m_state) {
      case BTN_IDLE:
         stopAnimation();
         playAnimation(hoverOnStr);
         m_state = BTN_HOVER_ON;
      break;
      case BTN_PRESS:
      case BTN_RELEASE:
      case BTN_HOVER_ON:
      case BTN_HOVER_OFF:
      break;
   }
}

//===========================================
// UiButton::onHoverOff
//===========================================
void UiButton::onHoverOff(float32_t x, float32_t y) {
   static long hoverOffStr = internString("hoverOff");
   static long releaseStr = internString("release");

   m_mouseOver = false;

   switch (m_state) {
      case BTN_PRESS:
         if (playAnimation(releaseStr))
            m_state = BTN_RELEASE;
      break;
      case BTN_RELEASE:
      case BTN_HOVER_ON:
      case BTN_HOVER_OFF:
      break;
      case BTN_IDLE:
         stopAnimation();
         playAnimation(hoverOffStr);
         m_state = BTN_HOVER_OFF;
      break;
   }
}


}
