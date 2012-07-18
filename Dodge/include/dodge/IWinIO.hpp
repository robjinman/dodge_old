/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __I_WIN_IO_HPP__
#define __I_WIN_IO_HPP__


#include <map>
#include <string>
#include <vector>
#include <boost/variant.hpp>
#include "utils/Functor.hpp"
#include "utils/TypeList.hpp"


namespace Dodge {


class IWinIO {
   public:

      // These are defaults for Linux.
      // Implementation should redefine these.
      enum keyCode_t {
         KEY_BACKSPACE = 65288,
         KEY_TAB = 65289,
         KEY_ENTER = 65293,
         KEY_SHIFT = 65506,
         KEY_CONTROL = 65508,
         KEY_ALT = 65513,
         KEY_PAUSE = 65299,
         KEY_CAPS = 65509,
         KEY_ESCAPE = 65307,
         KEY_SPACE = 32,
         KEY_PAGEUP = 65365,
         KEY_PAGEDOWN = 65366,
         KEY_END = 65367,
         KEY_HOME = 65360,
         KEY_LEFT = 65361,
         KEY_UP = 65362,
         KEY_RIGHT = 65363,
         KEY_DOWN = 65364,
         KEY_PRINTSCREEN = 64797,
         KEY_INSERT = 65379,
         KEY_DELETE = 65535,
         KEY_0 = 48,
         KEY_1 = 49,
         KEY_2 = 50,
         KEY_3 = 51,
         KEY_4 = 52,
         KEY_5 = 53,
         KEY_6 = 54,
         KEY_7 = 55,
         KEY_8 = 56,
         KEY_9 = 57,
         KEY_A = 97,
         KEY_B = 98,
         KEY_C = 99,
         KEY_D = 100,
         KEY_E = 101,
         KEY_F = 102,
         KEY_G = 103,
         KEY_H = 104,
         KEY_I = 105,
         KEY_J = 106,
         KEY_K = 107,
         KEY_L = 108,
         KEY_M = 109,
         KEY_N = 110,
         KEY_O = 111,
         KEY_P = 112,
         KEY_Q = 113,
         KEY_R = 114,
         KEY_S = 115,
         KEY_T = 116,
         KEY_U = 117,
         KEY_V = 118,
         KEY_W = 119,
         KEY_X = 120,
         KEY_Y = 121,
         KEY_Z = 122,
         KEY_LWIN = 65515,
         KEY_RWIN = 65516,
         KEY_APPS = 65383,
         KEY_NUMPAD0 = 65438,
         KEY_NUMPAD1 = 65436,
         KEY_NUMPAD2 = 65433,
         KEY_NUMPAD3 = 65435,
         KEY_NUMPAD4 = 65430,
         KEY_NUMPAD5 = 65437,
         KEY_NUMPAD6 = 65432,
         KEY_NUMPAD7 = 65429,
         KEY_NUMPAD8 = 65431,
         KEY_NUMPAD9 = 65434,
         KEY_MULTIPLY = 65450,
         KEY_ADD = 65451,
         KEY_SUBTRACT = 65453,
         KEY_DECIMAL = 65439,
         KEY_DIVIDE = 65455,
         KEY_F1 = 65470,
         KEY_F2 = 65471,
         KEY_F3 = 65472,
         KEY_F4 = 65473,
         KEY_F5 = 65474,
         KEY_F6 = 65475,
         KEY_F7 = 65476,
         KEY_F8 = 65477,
         KEY_F9 = 65478,
         KEY_F10 = 65479,
         KEY_F11 = 65480,
         KEY_F12 = 65481,
         KEY_NUMLOCK = 65407,
         KEY_SCROLL = 65300,
         KEY_LSHIFT = 65505,
         KEY_RSHIFT = 65506,
         KEY_LCONTROL = 65507,
         KEY_RCONTROL = 65508,
         KEY_LMENU = 65515,
         KEY_RMENU = 65516
      };

      enum winEvent_t {
         EVENT_UNDEFINED,
         EVENT_WINCLOSE,
         EVENT_WINEXPOSE,
         EVENT_WINRESIZE,
         EVENT_KEYDOWN,
         EVENT_KEYUP,
         EVENT_MOUSEMOVE,
         EVENT_BTN1PRESS,
         EVENT_BTN2PRESS,
         EVENT_BTN3PRESS,
         EVENT_BTN1RELEASE,
         EVENT_BTN2RELEASE,
         EVENT_BTN3RELEASE
      };

      typedef boost::variant <
         Functor<void, TYPELIST_0()>,
         Functor<void, TYPELIST_1(int)>,
         Functor<void, TYPELIST_2(int, int)>
      > callback_t;

      // Construct window and rendering context
      virtual void init(const std::string& winTitle, int w, int h, bool fullscreen = false) = 0;

      // Associate callback function with window/io event. Multiple callbacks per event are permitted.
      virtual void registerCallback(winEvent_t event, callback_t func);

      virtual void unregisterCallback(winEvent_t event, callback_t func);

      // Dispatch events to their respective callback functions
      virtual void doEvents() = 0;

      virtual void destroyWindow() = 0;

      inline int getWindowWidth() const;

      inline int getWindowHeight() const;

      virtual void swapBuffers() = 0;

   protected:
      typedef std::vector<callback_t> callbackList_t;
      typedef std::map<winEvent_t, callbackList_t> callbackMap_t;
      static callbackMap_t m_callbacks;

      static int m_width;
      static int m_height;
};

//===========================================
// IWinIO::getWindowHeight
//===========================================
inline int IWinIO::getWindowHeight() const {
   return m_height;
}

//===========================================
// IWinIO::registerCallback
//===========================================
inline void IWinIO::registerCallback(winEvent_t event, callback_t func) {
   m_callbacks[event].push_back(func);
}


}


#endif /*!__I_WIN_IO_HPP__*/
