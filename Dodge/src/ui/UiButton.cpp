/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <ui/UiButton.hpp>
#include <StringId.hpp>


using namespace std;


namespace Dodge {


#ifdef DEBUG
//===========================================
// UiButton::dbg_print
//===========================================
void UiButton::dbg_print(ostream& out, int tab) const {
   // TODO
}
#endif

//===========================================
// UiButton::getSize
//===========================================
size_t UiButton::getSize() const {
   return sizeof(UiButton)
        - sizeof(EntityUi)
        - sizeof(Sprite)
        + Sprite::getSize()
        + EntityUi::getSize();
}

//===========================================
// UiButton::clone
//===========================================
UiButton* UiButton::clone() const {
   return new UiButton(*this);
}

//===========================================
// UiButton::assignData
//===========================================
void UiButton::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, UiButton);

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "Sprite") {
         Sprite::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class UiButton; ");
      throw;
   }
}

//===========================================
// UiButton::update
//===========================================
void UiButton::update() {
   static long noFocusIdleStr = internString("noFocusIdle");
   static long focusIdleStr = internString("focusIdle");
   static long releaseStr = internString("release");

   Sprite::update();
   EntityUi::update();

   if (getAnimState() == Animation::STOPPED) {
      switch (m_state) {
         case LOSE_FOCUS:
         case NO_FOCUS_IDLE:
            playAnimation(noFocusIdleStr);
         break;
         case GAIN_FOCUS:
         case FOCUS_IDLE:
            playAnimation(focusIdleStr);
         break;
         case RELEASE:
            m_onRelease(shared_from_this());
            m_state = FOCUS_IDLE;
         break;
         case PRESS:
            m_onClick(shared_from_this());
            if (!m_btn1Pressed || !m_mouseOver) {
               playAnimation(releaseStr);
               m_state = RELEASE;
            }
         break;
      }
   }
}

//===========================================
// UiButton::onGainFocus
//===========================================
void UiButton::onGainFocus() {
   static long gainFocusStr = internString("gainFocus");

   switch (m_state) {
      case RELEASE:
      case LOSE_FOCUS:
      case NO_FOCUS_IDLE:
         stopAnimation();
         playAnimation(gainFocusStr);
         m_state = GAIN_FOCUS;
      break;
      case PRESS:
      case GAIN_FOCUS:
      case FOCUS_IDLE:
      break;
   }
}

//===========================================
// UiButton::onLoseFocus
//===========================================
void UiButton::onLoseFocus() {
   static long loseFocusStr = internString("loseFocus");

   switch (m_state) {
      case PRESS:
      case GAIN_FOCUS:
      case FOCUS_IDLE:
      case RELEASE:
         stopAnimation();
         playAnimation(loseFocusStr);
         m_state = LOSE_FOCUS;
      break;
      case LOSE_FOCUS:
      case NO_FOCUS_IDLE:
      break;
   }
}

//===========================================
// UiButton::onBtn1Press
//===========================================
void UiButton::onBtn1Press(float32_t x, float32_t y) {
   static long pressStr = internString("press");

   m_btn1Pressed = true;

   switch (m_state) {
      case PRESS:
      break;
      case RELEASE:
      case GAIN_FOCUS:
      case LOSE_FOCUS:
      case FOCUS_IDLE:
      case NO_FOCUS_IDLE:
         stopAnimation();
         playAnimation(pressStr);
         m_state = PRESS;
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
      case NO_FOCUS_IDLE:
      case RELEASE:
      case GAIN_FOCUS:
      case LOSE_FOCUS:
      case FOCUS_IDLE:
      break;
      case PRESS:
         if (playAnimation(releaseStr)) {
            m_state = RELEASE;
         }
      break;
   }
}

//===========================================
// UiButton::onKeyDown
//===========================================
void UiButton::onKeyDown(int key) {
   switch (key) {
      case WinIO::KEY_ENTER: onBtn1Press(0.f, 0.f); break;
      // ...
   }
}

//===========================================
// UiButton::onKeyUp
//===========================================
void UiButton::onKeyUp(int key) {
   switch (key) {
      case WinIO::KEY_ENTER: onBtn1Release(0.f, 0.f); break;
      // ...
   }
}

//===========================================
// UiButton::onHoverOn
//===========================================
void UiButton::onHoverOn(float32_t x, float32_t y) {
   m_mouseOver = true;
}

//===========================================
// UiButton::onHoverOff
//===========================================
void UiButton::onHoverOff(float32_t x, float32_t y) {
   m_mouseOver = false;
}


}
