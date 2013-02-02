/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <WinIO.hpp>
#include <Exception.hpp>


using namespace std;


namespace Dodge {


WinIO::callbackMap_t WinIO::m_callbacks = WinIO::callbackMap_t();
int WinIO::m_width = 0;
int WinIO::m_height = 0;

bool WinIO::m_init = false;
#ifdef DEBUG
byte_t WinIO::dbg_flags = 0;
#endif


void WinIO::init(const std::string& winTitle, int w, int h, bool fullscreen) {
   // create native window
   EGLNativeDisplayType nativeDisplay = (EGLNativeDisplayType)NULL;

   // get egl display handle
   m_eglDisplay = eglGetDisplay(nativeDisplay);

   if(m_eglDisplay == EGL_NO_DISPLAY)
      throw Exception("Could not create EGL display", __FILE__, __LINE__);

   // Initialize the display
   EGLint major = 0;
   EGLint minor = 0;

   if (!eglInitialize(m_eglDisplay, &major, &minor))
      throw Exception("Could not initialize EGL display", __FILE__, __LINE__);

   if (major < 1 || minor < 4)
      throw Exception("System does not support at least EGL 1.4", __FILE__, __LINE__);

   // Obtain the first configuration with a depth buffer
   EGLint attrs[] = { EGL_DEPTH_SIZE, 16, EGL_NONE };
   EGLint numConfig =0;

   if (!eglChooseConfig(m_eglDisplay, attrs, &m_eglConfig, 1, &numConfig))
      throw Exception("Could not find valid EGL config", __FILE__, __LINE__);

   createWindow(winTitle, w, h, fullscreen);

   // Create a surface for the main window
   m_eglSurface = eglCreateWindowSurface(m_eglDisplay,m_eglConfig, m_win, NULL);

   if (m_eglSurface == EGL_NO_SURFACE) {
      destroyWindow();
      throw Exception("Could not create EGL surface", __FILE__, __LINE__);
   }
}

void WinIO::createGLContext() {
   // Create an OpenGL ES context
   EGLContext eglContext;
   eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, NULL);

   if (eglContext == EGL_NO_CONTEXT) {
      destroyWindow();
      throw Exception("Could not create EGL context", __FILE__, __LINE__);
   }

   // Make the context and surface current
   if(!eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, eglContext)) {
      destroyWindow();
      throw Exception("Could not activate EGL context", __FILE__, __LINE__);
   }
}

void WinIO::createWindow(const std::string& winTitle, int w, int h, bool fullscreen) {
   HINSTANCE hInstance = GetModuleHandle(NULL);
   HWND hWnd = NULL;
   DWORD dwExtStyle;
   DWORD dwWindStyle;

   TCHAR szWindowName[50] =  TEXT("OpenGL ES Sample");
   TCHAR szClassName[50]  =  TEXT("OGL_CLASS");

   // setup window class
   WNDCLASS wndClass;
   wndClass.lpszClassName = szClassName;                          // Set the name of the Class
   wndClass.lpfnWndProc   = (WNDPROC)WinIO::wndProc;
   wndClass.hInstance     = hInstance;                            // Use this module for the module handle
   wndClass.hCursor       = LoadCursor(NULL, IDC_ARROW);          // Pick the default mouse cursor
   wndClass.hIcon         = LoadIcon(NULL, IDI_WINLOGO);          // Pick the default windows icons
   wndClass.hbrBackground = NULL;                                 // No Background
   wndClass.lpszMenuName  = NULL;                                 // No menu for this window
   wndClass.style         = CS_HREDRAW | CS_OWNDC | CS_VREDRAW;   // set styles for this class, specifically to catch
                                                                  // window redraws, unique DC, and resize
   wndClass.cbClsExtra    = 0;                                    // Extra class memory
   wndClass.cbWndExtra    = 0;                                    // Extra window memory

   // Register the newly defined class
   if(!RegisterClass(&wndClass))
      throw Exception("", __FILE__, __LINE__); // TODO

   dwExtStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
   dwWindStyle = WS_OVERLAPPEDWINDOW;
   ShowCursor(TRUE);

   RECT windowRect;
   windowRect.left   = 0;
   windowRect.right  = w;
   windowRect.top    = 0;
   windowRect.bottom = h;

   // Setup window width and height
   AdjustWindowRectEx(&windowRect, dwWindStyle, FALSE, dwExtStyle);

   //Adjust for adornments
   int nWindowWidth  = windowRect.right  - windowRect.left;
   int nWindowHeight = windowRect.bottom - windowRect.top;

   // Create window
   hWnd = CreateWindowEx(
      dwExtStyle,                                        // Extended style
      szClassName,                                       // class name
      szWindowName,                                      // window name
      dwWindStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,   // window stlye
      0,                                                 // window position, x
      0,                                                 // window position, y
      nWindowWidth,                                      // height
      nWindowHeight,                                     // width
      NULL,                                              // Parent window
      NULL,                                              // menu
      hInstance,                                         // instance
      NULL                                               // pass this to WM_CREATE
   );

   ShowWindow(hWnd, SW_SHOWDEFAULT);

   m_win = (EGLNativeWindowType) hWnd;
}

void WinIO::swapBuffers() {
   eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

void WinIO::destroyWindow() {
    WINDOWINFO info;
    GetWindowInfo((HWND) m_win, &info);
    DestroyWindow((HWND) m_win);
    UnregisterClass((LPCTSTR) info.atomWindowType, GetModuleHandle(NULL));
}

LRESULT CALLBACK WinIO::wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
   try {
      switch (uMsg) {
         case WM_SYSCOMMAND:        // Intercept System Commands
         {
            switch (wParam)         // Check System Calls
            {
               case SC_SCREENSAVE:  // Screensaver Trying To Start?
               case SC_MONITORPOWER:// Monitor Trying To Enter Powersave?
               return 0;            // Prevent From Happening
            }
            break; // Exit
         }
         case WM_CLOSE: {
//            PostQuitMessage(0); // Send A Quit Message
            callbackMap_t::iterator it = m_callbacks.find(EVENT_WINCLOSE);
            if (it != m_callbacks.end()) {
               for (uint_t f = 0; f < it->second.size(); ++f)
                  boost::get<Functor<void, TYPELIST_0()> >(it->second[f])();
            }
            return 0;
         }
         case WM_KEYDOWN: {
            callbackMap_t::iterator it = m_callbacks.find(EVENT_KEYDOWN);
            if (it != m_callbacks.end()) {
               for (uint_t f = 0; f < it->second.size(); ++f)
                  boost::get<Functor<void, TYPELIST_1(int)> >(it->second[f])(wParam);
            }
            return 0;
         }
         case WM_KEYUP: {
            callbackMap_t::iterator it = m_callbacks.find(EVENT_KEYUP);
            if (it != m_callbacks.end()) {
               for (uint_t f = 0; f < it->second.size(); ++f)
                  boost::get<Functor<void, TYPELIST_1(int)> >(it->second[f])(wParam);
            }
            return 0;
         }
         case WM_LBUTTONDOWN: {
            callbackMap_t::iterator it = m_callbacks.find(EVENT_BTN1PRESS);
            if (it != m_callbacks.end()) {
               for (uint_t f = 0; f < it->second.size(); ++f)
                  boost::get<Functor<void, TYPELIST_2(int, int)> >(it->second[f])(LOWORD(lParam), HIWORD(lParam));
            }
            return 0;
         }
         case WM_MBUTTONDOWN: {
            callbackMap_t::iterator it = m_callbacks.find(EVENT_BTN2PRESS);
            if (it != m_callbacks.end()) {
               for (uint_t f = 0; f < it->second.size(); ++f)
                  boost::get<Functor<void, TYPELIST_2(int, int)> >(it->second[f])(LOWORD(lParam), HIWORD(lParam));
            }
            return 0;
         }
         case WM_RBUTTONDOWN: {
            callbackMap_t::iterator it = m_callbacks.find(EVENT_BTN3PRESS);
            if (it != m_callbacks.end()) {
               for (uint_t f = 0; f < it->second.size(); ++f)
                  boost::get<Functor<void, TYPELIST_2(int, int)> >(it->second[f])(LOWORD(lParam), HIWORD(lParam));
            }
            return 0;
         }
         case WM_SIZE: {
            if (m_width != LOWORD(lParam) || m_height != HIWORD(lParam)) {

               m_width = LOWORD(lParam);
               m_height = HIWORD(lParam);

               callbackMap_t::iterator it = m_callbacks.find(EVENT_WINRESIZE);
               if (it != m_callbacks.end()) {
                  for (uint_t f = 0; f < it->second.size(); ++f)
                     boost::get<Functor<void, TYPELIST_2(int, int)> >(it->second[f])(LOWORD(lParam), HIWORD(lParam));
               }
            }
            return 0;
         }
      }
   }
   catch (boost::bad_get& e) {
      Exception ex("Bad callback function; ", __FILE__, __LINE__);
      ex.append(e.what());
      throw ex;
   }
   catch (bad_alloc& e) {
      Exception ex("Error in WinIO::doEvents; ", __FILE__, __LINE__);
      ex.append(e.what());
      throw ex;
   }

   // Pass All Unhandled Messages To DefWindowProc
   return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

void WinIO::doEvents() {
   MSG msg;

   while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
/*
      if (msg.message == WM_QUIT) {
         map<GfxEvent_t, funcPtr_t>::iterator it = m_callbacks.find(GFXEVENT_WINCLOSE);
         if (it != m_callbacks.end()) {
            boost::get<void(*)()>(it->second)();
         }
         else {
            destroyWindow();
            exit(0);
         }
      }
      else {*/
         TranslateMessage(&msg);
         DispatchMessage(&msg);
//      }
   }
}


}
