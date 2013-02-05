/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <ui/UiButton.hpp>
#include <StringId.hpp>


using namespace std;


namespace Dodge {


//===========================================
// UiButton::UiButton
//===========================================
UiButton::UiButton(const XmlNode data)
   : Asset(internString("UiButton")),
     Entity(data.firstChild().firstChild()),
     Sprite(data.firstChild()),
     EntityUi(this),
     m_state(NO_FOCUS_IDLE),
     m_mouseOver(false),
     m_btn1Pressed(false),
     m_pressAndHold(false),
     m_onClick(&UiButton::void_entityPtr),
     m_onRelease(&UiButton::void_entityPtr) {

   try {
      XML_NODE_CHECK(data, UiButton);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class UiButton; ");
      throw;
   }

   m_activationKeys.insert(WinIO::KEY_ENTER);
}

//===========================================
// UiButton::UiButton
//===========================================
UiButton::UiButton(long type, pTexture_t texture)
   : Asset(internString("UiButton")),
     Entity(type),
     Sprite(type, texture),
     EntityUi(this),
     m_state(NO_FOCUS_IDLE),
     m_mouseOver(false),
     m_btn1Pressed(false),
     m_pressAndHold(false),
     m_onClick(&UiButton::void_entityPtr),
     m_onRelease(&UiButton::void_entityPtr) {

   m_activationKeys.insert(WinIO::KEY_ENTER);
}

//===========================================
// UiButton::UiButton
//===========================================
UiButton::UiButton(long name, long type, pTexture_t texture)
   : Asset(internString("UiButton")),
     Entity(name, type),
     Sprite(name, type, texture),
     EntityUi(this),
     m_state(NO_FOCUS_IDLE),
     m_mouseOver(false),
     m_btn1Pressed(false),
     m_pressAndHold(false),
     m_onClick(&UiButton::void_entityPtr),
     m_onRelease(&UiButton::void_entityPtr) {

   m_activationKeys.insert(WinIO::KEY_ENTER);
}

//===========================================
// UiButton::UiButton
//===========================================
UiButton::UiButton(const UiButton& copy)
   : Asset(internString("UiButton")),
     Entity(copy),
     Sprite(copy),
     EntityUi(copy, this),
     m_state(NO_FOCUS_IDLE),
     m_mouseOver(false),
     m_btn1Pressed(false),
     m_pressAndHold(false),
     m_activationKeys(copy.m_activationKeys),
     m_onClick(&UiButton::void_entityPtr),
     m_onRelease(&UiButton::void_entityPtr) {

   m_activationKeys.insert(WinIO::KEY_ENTER);
}

//===========================================
// UiButton::UiButton
//===========================================
UiButton::UiButton(const UiButton& copy, long name)
   : Asset(internString("UiButton")),
     Entity(copy, name),
     Sprite(copy, name),
     EntityUi(copy, this),
     m_state(NO_FOCUS_IDLE),
     m_mouseOver(false),
     m_btn1Pressed(false),
     m_pressAndHold(false),
     m_activationKeys(copy.m_activationKeys),
     m_onClick(&UiButton::void_entityPtr),
     m_onRelease(&UiButton::void_entityPtr) {

   m_activationKeys.insert(WinIO::KEY_ENTER);
}

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
Asset* UiButton::clone() const {
   return new UiButton(*this);
}

//===========================================
// UiButton::addToWorld
//===========================================
void UiButton::addToWorld() {
   Sprite::addToWorld();
   EntityUi::addToWorld();
}

//===========================================
// UiButton::removeFromWorld
//===========================================
void UiButton::removeFromWorld() {
   Sprite::removeFromWorld();
   EntityUi::removeFromWorld();
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
   static long pressStr = internString("press");

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

            if (!m_mouseOver && !m_pressAndHold)
               onBtn1Release(0, 0);

            if (!hasAnimation(pressStr))
               playAnimation(focusIdleStr);
         break;
      }
   }
}

//===========================================
// UiButton::press
//===========================================
void UiButton::press() {
   onBtn1Press(0, 0);
}

//===========================================
// UiButton::pressAndHold
//===========================================
void UiButton::pressAndHold() {
   onBtn1Press(0, 0);
   m_pressAndHold = true;
}

//===========================================
// UiButton::release
//===========================================
void UiButton::release() {
   onBtn1Release(0, 0);
   m_pressAndHold = false;
}

//===========================================
// UiButton::isPressed
//===========================================
bool UiButton::isPressed() const {
   return m_state == PRESS;
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
         if (hasAnimation(gainFocusStr)) {
            stopAnimation();
            playAnimation(gainFocusStr);
         }
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
         if (hasAnimation(loseFocusStr)) {
            stopAnimation();
            playAnimation(loseFocusStr);

            if (m_state == RELEASE) m_onRelease(shared_from_this());
         }
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
         if (hasAnimation(pressStr)) {
            stopAnimation();
            playAnimation(pressStr);

            if (m_state == RELEASE) m_onRelease(shared_from_this());
         }
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
         stopAnimation();
         playAnimation(releaseStr);
         m_state = RELEASE;
      break;
   }
}

//===========================================
// UiButton::onKeyDown
//===========================================
void UiButton::onKeyDown(int key) {
   if (m_activationKeys.find(key) != m_activationKeys.end()) {
      if (!m_pressAndHold)
         pressAndHold();
   }
}

//===========================================
// UiButton::onKeyUp
//===========================================
void UiButton::onKeyUp(int key) {
   if (m_activationKeys.find(key) != m_activationKeys.end())
      release();
}

//===========================================
// UiButton::addActivationKey
//===========================================
void UiButton::addActivationKey(int key) {
   m_activationKeys.insert(key);
}

//===========================================
// UiButton::removeActivationKey
//===========================================
void UiButton::removeActivationKey(int key) {
   m_activationKeys.erase(key);
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
