/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <ui/UiCheckbox.hpp>
#include <StringId.hpp>


namespace Dodge {


//===========================================
// UiCheckbox::update
//===========================================
void UiCheckbox::update() {
   static long idleTrueStr = internString("idleTrue");
   static long idleFalseStr = internString("idleFalse");
   static long hoverOffStr = internString("hoverOff");
   static long hoverOnStr = internString("hoverOn");
   static long releaseStr = internString("release");

   Sprite::update();

   if (getAnimState() == Animation::STOPPED) {
      switch (m_state) {
         case CHK_IDLE:
            playAnimation(m_checked ? idleTrueStr : idleFalseStr);
         break;
         case CHK_HOVER_OFF:
            if (m_mouseOver) {
               playAnimation(m_checked ? hoverOnTrueStr : hoverOnFalseStr);
               m_state = CHK_HOVER_ON;
            }
         break;
         case CHK_HOVER_ON:
            if (!m_mouseOver) {
               playAnimation(m_checked ? hoverOffTrueStr : hoverOffFalseStr);
               m_state = CHK_HOVER_OFF;
            }
         break;
         case CHK_RELEASE:
            if (m_mouseOver)
               m_state = CHK_HOVER_ON;
            else {
               playAnimation(m_checked ? hoverOffTrueStr : hoverOffFalseStr);
               m_state = CHK_HOVER_OFF;
            }
         break;
         case CHK_PRESS:
            if (!m_btn1Pressed || !m_mouseOver) {
               playAnimation(releaseStr);
               m_state = CHK_RELEASE;
            }
         break;
      }
   }
}

//===========================================
// UiCheckbox::onBtn1Press
//===========================================
void UiCheckbox::onBtn1Press(float32_t x, float32_t y) {
   static long pressStr = internString("press");

   m_btn1Pressed = true;

   switch (m_state) {
      case CHK_PRESS:
      case CHK_RELEASE:
      break;
      case CHK_HOVER_ON:
      case CHK_HOVER_OFF:
      case CHK_IDLE:
         stopAnimation();
         playAnimation(pressStr);
         m_state = CHK_PRESS;
         m_onClick(shared_from_this());
      break;
   }
}

//===========================================
// UiCheckbox::onBtn1Release
//===========================================
void UiCheckbox::onBtn1Release(float32_t x, float32_t y) {
   static long releaseStr = internString("release");

   m_btn1Pressed = false;

   switch (m_state) {
      case CHK_IDLE:
      case CHK_RELEASE:
      case CHK_HOVER_ON:
      case CHK_HOVER_OFF:
      break;
      case CHK_PRESS:
         if (playAnimation(releaseStr)) {
            m_state = CHK_RELEASE;
            m_onRelease(shared_from_this());
         }
      break;
   }
}

//===========================================
// UiCheckbox::onHoverOn
//===========================================
void UiCheckbox::onHoverOn(float32_t x, float32_t y) {
   static long hoverOnStr = internString("hoverOn");

   m_mouseOver = true;

   switch (m_state) {
      case CHK_IDLE:
         stopAnimation();
         playAnimation(hoverOnStr);
         m_state = CHK_HOVER_ON;
      break;
      case CHK_PRESS:
      case CHK_RELEASE:
      case CHK_HOVER_ON:
      case CHK_HOVER_OFF:
      break;
   }
}

//===========================================
// UiCheckbox::onHoverOff
//===========================================
void UiCheckbox::onHoverOff(float32_t x, float32_t y) {
   static long hoverOffStr = internString("hoverOff");
   static long releaseStr = internString("release");

   m_mouseOver = false;

   switch (m_state) {
      case CHK_PRESS:
         if (playAnimation(releaseStr))
            m_state = CHK_RELEASE;
      break;
      case CHK_RELEASE:
      case CHK_HOVER_ON:
      case CHK_HOVER_OFF:
      break;
      case CHK_IDLE:
         stopAnimation();
         playAnimation(hoverOffStr);
         m_state = CHK_HOVER_OFF;
      break;
   }
}


}
