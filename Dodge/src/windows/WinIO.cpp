/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2011
 */

#include <map>
#include <string>
#include "WinIO.hpp"
#include "Exception.hpp"


using namespace std;


namespace Dodge {


WinIO::callbackMap_t WinIO::m_callbacks = map<winEvent_t, callbackList_t>();

HDC WinIO::m_hDC = NULL;
HGLRC WinIO::m_hRC = NULL;
HWND WinIO::m_hWnd = NULL;
HINSTANCE WinIO::m_hInstance = NULL;
bool WinIO::m_fullscreen = false;
int WinIO::m_width;
int WinIO::m_height;
bool WinIO::m_init = false;
#ifdef DEBUG
byte_t WinIO::dbg_flags = 0;
#endif


//===========================================
// WinIO::init
//===========================================
void WinIO::init(const std::string& winTitle, int w, int h, bool fullscreen) {
   m_width = w;
   m_height = h;

   createWindow(winTitle.data(), w, h, fullscreen);

   m_init = true;
}

//===========================================
// WinIO::createGLWindow
//===========================================
void WinIO::createWindow(const string& title, int width, int height, bool fullscreenflag) {
   GLuint      bits = 32;
   GLuint      PixelFormat;   // Holds The Results After Searching For A Match
   WNDCLASS    wc;            // Windows Class Structure
   DWORD       dwExStyle;     // Window Extended Style
   DWORD       dwStyle;       // Window Style
   RECT        WindowRect;    // Grabs Rectangle Upper Left / Lower Right Values

   WindowRect.left = static_cast<long>(0);         // Set Left Value To 0
   WindowRect.right = static_cast<long>(width);    // Set Right Value To Requested Width
   WindowRect.top = static_cast<long>(0);          // Set Top Value To 0
   WindowRect.bottom = static_cast<long>(height);  // Set Bottom Value To Requested Height

   m_fullscreen = fullscreenflag;   // Set The Global Fullscreen Flag

   m_hInstance       = GetModuleHandle(NULL);               // Grab An Instance For Our Window
   wc.style          = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;  // Redraw On Size, And Own DC For Window.
   wc.lpfnWndProc    = static_cast<WNDPROC>(wndProc);       // WndProc Handles Messages
   wc.cbClsExtra     = 0;                                   // No Extra Window Data
   wc.cbWndExtra     = 0;                                   // No Extra Window Data
   wc.hInstance      = m_hInstance;                         // Set The Instance
   wc.hIcon          = LoadIcon(NULL, IDI_WINLOGO);         // Load The Default Icon
   wc.hCursor        = LoadCursor(NULL, IDC_ARROW);         // Load The Arrow Pointer
   wc.hbrBackground  = NULL;                                // No Background Required For GL
   wc.lpszMenuName   = NULL;                                // We Don't Want A Menu
   wc.lpszClassName  = "OpenGL";                            // Set The Class Name

   if (!RegisterClass(&wc))
      throw Exception("Error creating GL window", __FILE__, __LINE__);

   if (m_fullscreen) {
      DEVMODE dmScreenSettings;                                // Device Mode
      memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));  // Makes Sure Memory's Cleared
      dmScreenSettings.dmSize = sizeof(dmScreenSettings);      // Size Of The Devmode Structure
      dmScreenSettings.dmPelsWidth = width;                    // Selected Screen Width
      dmScreenSettings.dmPelsHeight = height;                  // Selected Screen Height
      dmScreenSettings.dmBitsPerPel = bits;                    // Selected Bits Per Pixel
      dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

      // Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
      if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
         throw Exception("Error creating GL window; fullscreen mode not supported", __FILE__, __LINE__);
   }

   if (m_fullscreen) {
      dwExStyle = WS_EX_APPWINDOW;  // Window Extended Style
      dwStyle = WS_POPUP;           // Windows Style
      ShowCursor(false);            // Hide Mouse Pointer
   }
   else {
      dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;    // Window Extended Style
      dwStyle = WS_OVERLAPPEDWINDOW;                     // Windows Style
   }

   // Adjust Window To True Requested Size
   AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);

   // Create The Window
   if (!(m_hWnd = CreateWindowEx(dwExStyle, "OpenGL", title.data(), dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0, 0,
      WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, NULL, NULL, m_hInstance, NULL))) {

      destroyWindow();
      throw Exception("Error creating GL window", __FILE__, __LINE__);
   }

   static PIXELFORMATDESCRIPTOR pfd = {   // pfd Tells Windows How We Want Things To Be
      sizeof(PIXELFORMATDESCRIPTOR),      // Size Of This Pixel Format Descriptor
      1,                                  // Version Number
      PFD_DRAW_TO_WINDOW |                // Format Must Support Window
      PFD_SUPPORT_OPENGL |                // Format Must Support OpenGL
      PFD_DOUBLEBUFFER,                   // Must Support Double Buffering
      PFD_TYPE_RGBA,                      // Request An RGBA Format
      bits,                               // Select Our Color Depth
      0, 0, 0, 0, 0, 0,                   // Color Bits Ignored
      0,                                  // No Alpha Buffer
      0,                                  // Shift Bit Ignored
      0,                                  // No Accumulation Buffer
      0, 0, 0, 0,                         // Accumulation Bits Ignored
      16,                                 // 16Bit Z-Buffer (Depth Buffer)  
      0,                                  // No Stencil Buffer
      0,                                  // No Auxiliary Buffer
      PFD_MAIN_PLANE,                     // Main Drawing Layer
      0,                                  // Reserved
      0, 0, 0                             // Layer Masks Ignored
   };

   if (!(m_hDC = GetDC(m_hWnd))) {
      destroyWindow();
      throw Exception("Error creating GL window", __FILE__, __LINE__);
   }

   if (!(PixelFormat = ChoosePixelFormat(m_hDC, &pfd))) {
      destroyWindow();
      throw Exception("Error creating GL window", __FILE__, __LINE__);
   }

   if (!SetPixelFormat(m_hDC, PixelFormat, &pfd)) {
      destroyWindow();
      throw Exception("Error creating GL window", __FILE__, __LINE__);
   }

   ShowWindow(m_hWnd, SW_SHOW);
   SetForegroundWindow(m_hWnd);
   SetFocus(m_hWnd);
}

//===========================================
// WinIO::createGLContext
//===========================================
void WinIO::createGLContext() {
   if (!(m_hRC = wglCreateContext(m_hDC))) {
      destroyWindow();
      throw Exception("Error creating GL context", __FILE__, __LINE__);
   }

   if (!wglMakeCurrent(m_hDC, m_hRC)) {
      destroyWindow();
      throw Exception("Error creating GL context", __FILE__, __LINE__);
   }

   if (glewInit() != GLEW_OK) {
      destroyWindow();
      throw Exception("Error creating GL context", __FILE__, __LINE__);
   }
}

//===========================================
// WinIO::isSupportedGLVersion
//===========================================
bool WinIO::isSupportedGLVersion(glVersion_t version) const {
   switch (version) {
      case GL_1_1:     return GLEW_VERSION_1_1;      break;
      case GL_1_2:     return GLEW_VERSION_1_2;      break;
      case GL_1_2_1:   return GLEW_VERSION_1_2_1;    break;
      case GL_1_3:     return GLEW_VERSION_1_3;      break;
      case GL_1_4:     return GLEW_VERSION_1_4;      break;
      case GL_1_5:     return GLEW_VERSION_1_5;      break;
      case GL_2_0:     return GLEW_VERSION_2_0;      break;
      case GL_2_1:     return GLEW_VERSION_2_1;      break;
      case GL_3_0:     return GLEW_VERSION_3_0;      break;
      case GL_3_1:     return GLEW_VERSION_3_1;      break;
      case GL_3_2:     return GLEW_VERSION_3_2;      break;
      case GL_3_3:     return GLEW_VERSION_3_3;      break;
      case GL_4_0:     return GLEW_VERSION_4_0;      break;
      case GL_4_1:     return GLEW_VERSION_4_1;      break;
      case GL_4_2:     return GLEW_VERSION_4_2;      break;
      case GL_4_3:     return GLEW_VERSION_4_3;      break;

      default: return false;
   };
}

//===========================================
// WinIO::destroyWindow
//===========================================
void WinIO::destroyWindow() {
   if (m_fullscreen) {
      ChangeDisplaySettings(NULL, 0);
      ShowCursor(true);
   }

   if (m_hRC) { // Do We Have A Rendering Context?
      wglMakeCurrent(NULL, NULL);
      wglDeleteContext(m_hRC);
   }

   if (m_hDC) ReleaseDC(m_hWnd,m_hDC);

   if (m_hWnd) DestroyWindow(m_hWnd);

   UnregisterClass("OpenGL",m_hInstance);

   m_init = false;
}

//===========================================
// WinIO::wndProc
//===========================================
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

//===========================================
// WinIO::doEvents
//===========================================
void WinIO::doEvents() {
   MSG msg;

   while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }
}


}
