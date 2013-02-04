/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __WIN_IO_HPP__
#define __WIN_IO_HPP__


#include <windows.h>
#include <windowsx.h>
#include <GLEW/glew.h>
#include <map>
#include <string>
#include <vector>
#include <boost/variant.hpp>
#include "../../utils/Functor.hpp"
#include "../../utils/TypeList.hpp"
#include "../definitions.hpp"


namespace Dodge {


class WinIO {
   friend class Renderer;

   public:
      enum keyCode_t {
         KEY_BACKSPACE = 0x08,
			KEY_TAB = 0x09,
			KEY_ENTER = 0x0D,
			KEY_SHIFT = 0x10,
			KEY_CONTROL = 0x11,
			KEY_ALT = 0x12,
			KEY_PAUSE = 0x13,
			KEY_CAPS = 0x14,
			KEY_ESCAPE = 0x1B,
			KEY_SPACE = 0x20,
			KEY_PAGEUP = 0x21,
			KEY_PAGEDOWN = 0x22,
			KEY_END = 0x23,
			KEY_HOME = 0x24,
			KEY_LEFT = 0x25,
			KEY_UP = 0x26,
			KEY_RIGHT = 0x27,
			KEY_DOWN = 0x28,
			KEY_PRINTSCREEN = 0x2C,
			KEY_INSERT = 0x2D,
			KEY_DELETE = 0x2E,
			KEY_0 = 0x30,
			KEY_1 = 0x31,
			KEY_2 = 0x32,
			KEY_3 = 0x33,
			KEY_4 = 0x34,
			KEY_5 = 0x35,
			KEY_6 = 0x36,
			KEY_7 = 0x37,
			KEY_8 = 0x38,
			KEY_9 = 0x39,
			KEY_A = 0x41,
			KEY_B = 0x42,
			KEY_C = 0x43,
			KEY_D = 0x44,
			KEY_E = 0x45,
			KEY_F = 0x46,
			KEY_G = 0x47,
			KEY_H = 0x48,
			KEY_I = 0x49,
			KEY_J = 0x4A,
			KEY_K = 0x4B,
			KEY_L = 0x4C,
			KEY_M = 0x4D,
			KEY_N = 0x4E,
			KEY_O = 0x4F,
			KEY_P = 0x50,
			KEY_Q = 0x51,
			KEY_R = 0x52,
			KEY_S = 0x53,
			KEY_T = 0x54,
			KEY_U = 0x55,
			KEY_V = 0x56,
			KEY_W = 0x57,
			KEY_X = 0x58,
			KEY_Y = 0x59,
			KEY_Z = 0x5A,
			KEY_LWIN = 0x5B,
			KEY_RWIN = 0x5C,
			KEY_APPS = 0x5D,
			KEY_NUMPAD0 = 0x60,
			KEY_NUMPAD1 = 0x61,
			KEY_NUMPAD2 = 0x62,
			KEY_NUMPAD3 = 0x63,
			KEY_NUMPAD4 = 0x64,
			KEY_NUMPAD5 = 0x65,
			KEY_NUMPAD6 = 0x66,
			KEY_NUMPAD7 = 0x67,
			KEY_NUMPAD8 = 0x68,
			KEY_NUMPAD9 = 0x69,
			KEY_MULTIPLY = 0x6A,
			KEY_ADD = 0x6B,
			KEY_SUBTRACT = 0x6D,
			KEY_DECIMAL = 0x6E,
			KEY_DIVIDE = 0x6F,
			KEY_F1 = 0x70,
			KEY_F2 = 0x71,
			KEY_F3 = 0x72,
			KEY_F4 = 0x73,
			KEY_F5 = 0x74,
			KEY_F6 = 0x75,
			KEY_F7 = 0x76,
			KEY_F8 = 0x77,
			KEY_F9 = 0x78,
			KEY_F10 = 0x79,
			KEY_F11 = 0x7A,
			KEY_F12 = 0x7B,
			KEY_NUMLOCK = 0x90,
			KEY_SCROLL = 0x91,
			KEY_LSHIFT = 0xA0,
			KEY_RSHIFT = 0xA1,
			KEY_LCONTROL = 0xA2,
			KEY_RCONTROL = 0xA3,
			KEY_LMENU = 0xA4,
			KEY_RMENU = 0xA5,
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

      void init(const std::string& winTitle, int w, int h, bool fullscreen = false);
      inline int getWindowWidth() const;
      inline int getWindowHeight() const;
      void registerCallback(winEvent_t event, callback_t func);
      void unregisterCallback(winEvent_t event, callback_t func);
      void doEvents();
      void destroyWindow();
      void swapBuffers();

#ifdef DEBUG
      static const byte_t DBG_NO_VSYNC = 1 << 0;

      static byte_t dbg_flags;
#endif

   private:
      enum glVersion_t {
         GL_1_1,
         GL_1_2,
         GL_1_2_1,
         GL_1_3,
         GL_1_4,
         GL_1_5,
         GL_2_0,
         GL_2_1,
         GL_3_0,
         GL_3_1,
         GL_3_2,
         GL_3_3,
         GL_4_0,
         GL_4_1,
         GL_4_2,
         GL_4_3
      };

      void createGLContext();
      bool isSupportedGLVersion(glVersion_t version) const;

      typedef std::vector<callback_t> callbackList_t;
      typedef std::map<winEvent_t, callbackList_t> callbackMap_t;
      static callbackMap_t m_callbacks;

      static int m_width;
      static int m_height;
      static bool m_fullscreen;

      static bool m_init;

      //-------------------------------------
      static HDC        m_hDC;         // Private GDI Device Context
      static HGLRC      m_hRC;         // Permanent Rendering Context
      static HWND       m_hWnd;        // Holds Our Window Handle
      static HINSTANCE  m_hInstance;   // Holds The Instance Of The Application

      static bool m_active;
      //-------------------------------------

      void createWindow(const std::string& winTitle, int w, int h, bool fullscreen);
      static LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

//===========================================
// WinIO::getWindowHeight
//===========================================
inline int WinIO::getWindowHeight() const {
   return m_height;
}

//===========================================
// WinIO::getWindowWidth
//===========================================
int WinIO::getWindowWidth() const {
   return m_width;
}

//===========================================
// WinIO::registerCallback
//===========================================
inline void WinIO::registerCallback(winEvent_t event, callback_t func) {
   m_callbacks[event].push_back(func);
}

inline void WinIO::swapBuffers() {
   SwapBuffers(m_hDC);
}


}


#endif /*!__WIN_IO_HPP__*/
