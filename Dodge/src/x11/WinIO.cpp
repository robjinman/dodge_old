/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <map>
#include <string>
#include <X11/XKBlib.h>
#include <x11/WinIO.hpp>


using namespace std;


namespace Dodge {


Display* WinIO::m_display = NULL;
Window WinIO::m_win = Window();
Colormap WinIO::m_colorMap = Colormap();
XVisualInfo* WinIO::m_pVisual = NULL;

EGLDisplay WinIO::m_eglDisplay = EGLDisplay();
EGLContext WinIO::m_eglContext = EGLContext();
EGLSurface WinIO::m_eglSurface = EGLSurface();

WinIO::callbackMap_t WinIO::m_callbacks = WinIO::callbackMap_t();
int WinIO::m_width = 0;
int WinIO::m_height = 0;

bool WinIO::m_init = false;


//===========================================
// WinIO::init
//
// Sets up a new OpenGL ES 2.0 window.
//===========================================
void WinIO::init(const std::string& winTitle, int w, int h, bool fullscreen) {
   if (fullscreen) throw Exception("Fullscreen mode not supported", __FILE__, __LINE__);

   m_width = w;
   m_height = h;

   EGLint aEGLAttributes[] = {
      EGL_RED_SIZE, 8,
      EGL_GREEN_SIZE, 8,
      EGL_BLUE_SIZE, 8,
      EGL_DEPTH_SIZE, 16,
      EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,  // For OpenGLES1.x, set to EGL_OPENGL_ES_BIT
      EGL_NONE
   };
   EGLint aEGLContextAttributes[] = {
      EGL_CONTEXT_CLIENT_VERSION, 2,            // For OpenGLES1.x, set to 1
      EGL_NONE
   };
   EGLConfig eglConfig[1];
   EGLint nConfigs;

   m_display = XOpenDisplay(NULL);
   if(m_display == NULL)
      throw Exception("Error constructing window; could not create X Display", __FILE__, __LINE__);

   m_eglDisplay = EGL_CHECK(eglGetDisplay(m_display));
   EGL_CHECK(eglInitialize(m_eglDisplay, NULL, NULL));
   EGL_CHECK(eglChooseConfig(m_eglDisplay, aEGLAttributes, eglConfig, 1, &nConfigs));

   m_win = createXWindow(winTitle.data(), w, h, m_display, m_eglDisplay, eglConfig[0], &m_colorMap, &m_pVisual);
   if (!m_win) {
      eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
      eglTerminate(m_eglDisplay);
      XCloseDisplay(m_display);

      throw Exception("Error constructing window; could not create X Window", __FILE__, __LINE__);
   }

   // This is necessary to ensure we receive an event when the window is closed.
   // The event's type will be ClientMessage.
   Atom wmDelete = XInternAtom(m_display, "WM_DELETE_WINDOW", True);
   XSetWMProtocols(m_display, m_win, &wmDelete, 1);

   m_eglSurface = EGL_CHECK(eglCreateWindowSurface(m_eglDisplay, eglConfig[0], m_win, 0));
   m_eglContext = EGL_CHECK(eglCreateContext(m_eglDisplay, eglConfig[0], EGL_NO_CONTEXT, aEGLContextAttributes));
   EGL_CHECK(eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext));

   // No VSync
   XSynchronize(m_display, False);
   eglSwapInterval(m_eglDisplay, 0);

   m_init = true;
}

//===========================================
// WinIO::unregisterCallback
//===========================================
void WinIO::unregisterCallback(winEvent_t event, callback_t func) {
   callbackMap_t::iterator i = m_callbacks.find(event);

   if (i != m_callbacks.end()) {

     for (unsigned int f = 0; f < i->second.size(); f++) {
        if (i->second[f] == func) {
           i->second.erase(i->second.begin() + f);
           --f;
        }
     }

   }
}

//===========================================
// WinIO::waitForMap
//
// Waits for map notification
//===========================================
Bool WinIO::waitForMap(Display* d, XEvent* e, char* win_ptr) {
   return e->type == MapNotify && e->xmap.window == *reinterpret_cast<Window*>(win_ptr) ? True : False;
}

//===========================================
// WinIO::createXWindow
//===========================================
Window WinIO::createXWindow(const char* title, int width, int height, Display* m_display,
   EGLDisplay sEGLDisplay, EGLConfig FBConfig, Colormap* pColormap, XVisualInfo** ppVisual) {

   long screen = DefaultScreen(m_display);

   int vID;
   eglGetConfigAttrib(sEGLDisplay, FBConfig, EGL_NATIVE_VISUAL_ID, &vID);

   int n;
   XVisualInfo tplate;
   tplate.visualid = vID;
   XVisualInfo* visual = XGetVisualInfo(m_display, VisualIDMask, &tplate, &n);

   Colormap colorMap = XCreateColormap(m_display, RootWindow(m_display, screen), visual->visual, AllocNone);

   XSetWindowAttributes wa;
   wa.colormap = colorMap;
   wa.background_pixel = 0xFFFFFFFF;
   wa.border_pixel = 0;
   wa.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask
      | ButtonReleaseMask | PointerMotionMask | Button1MotionMask
      | Button2MotionMask | ExposureMask | StructureNotifyMask;

   unsigned long mask = CWBackPixel | CWBorderPixel | CWEventMask | CWColormap;

   Window window = XCreateWindow(m_display, RootWindow(m_display, screen), 0, 0, width, height,
      0, visual->depth, InputOutput, visual->visual, mask, &wa);

   XSizeHints sh;
   sh.flags = USPosition;
   sh.x = 10;
   sh.y = 10;
   XSetStandardProperties(m_display, window, title, title, None, 0, 0, &sh);
   XMapWindow(m_display, window);

   // Halt until window is "mapped".
   XEvent e;
   XIfEvent(m_display, &e, waitForMap, reinterpret_cast<XPointer>(&window));

   XSetWMColormapWindows(m_display, window, &window, 1);
   XFlush(m_display);

   *pColormap = colorMap;
   *ppVisual = visual;

   return window;
}

//===========================================
// WinIO::swapBuffers
//===========================================
void WinIO::swapBuffers() {
   eglSwapBuffers(m_eglDisplay, m_eglSurface);
}

//===========================================
// WinIO::doEvents
//
// Listens for GUI events, and calls the relevent
// callback functions.
//===========================================
void WinIO::doEvents() {
   try {
     while (XPending(m_display)) {
       XEvent xEvent;
       XNextEvent(m_display, &xEvent);

       switch (xEvent.type) {
         case Expose: {
            callbackMap_t::iterator it = m_callbacks.find(EVENT_WINEXPOSE);
            if (it != m_callbacks.end()) {
              for (callbackList_t::iterator f = it->second.begin(); f != it->second.end(); ++f)
                boost::get<Functor<void, TYPELIST_0()> >(*f)();
            }
         }
         break;
         case KeyPress: {
            callbackMap_t::iterator it = m_callbacks.find(EVENT_KEYDOWN);
            if (it != m_callbacks.end()) {
              for (callbackList_t::iterator f = it->second.begin(); f != it->second.end(); ++f)
                boost::get<Functor<void, TYPELIST_1(int)> >(*f)(XkbKeycodeToKeysym(m_display, xEvent.xkey.keycode, 0, 0));
            }
         }
         break;
         case KeyRelease: {
            callbackMap_t::iterator it = m_callbacks.find(EVENT_KEYUP);
            if (it != m_callbacks.end()) {
              for (callbackList_t::iterator f = it->second.begin(); f != it->second.end(); ++f)
                boost::get<Functor<void, TYPELIST_1(int)> >(*f)(XkbKeycodeToKeysym(m_display, xEvent.xkey.keycode, 0, 0));
            }
         }
         break;
         case ButtonPress: {
            winEvent_t kind = EVENT_UNDEFINED;
            switch (xEvent.xbutton.button) {
              case 1: kind = EVENT_BTN1PRESS; break;
              case 2: kind = EVENT_BTN2PRESS; break;
              case 3: kind = EVENT_BTN3PRESS;
            }
            callbackMap_t::iterator it = m_callbacks.find(kind);
            if (it != m_callbacks.end()) {
              for (callbackList_t::iterator f = it->second.begin(); f != it->second.end(); ++f)
                boost::get<Functor<void, TYPELIST_2(int, int)> >(*f)(xEvent.xbutton.x, xEvent.xbutton.y);
            }
         }
         break;
         case ButtonRelease: {
            winEvent_t kind = EVENT_UNDEFINED;
            switch (xEvent.xbutton.button) {
              case 1: kind = EVENT_BTN1RELEASE; break;
              case 2: kind = EVENT_BTN2RELEASE; break;
              case 3: kind = EVENT_BTN3RELEASE;
            }
            callbackMap_t::iterator it = m_callbacks.find(kind);
            if (it != m_callbacks.end()) {
              for (callbackList_t::iterator f = it->second.begin(); f != it->second.end(); ++f)
                boost::get<Functor<void, TYPELIST_2(int, int)> >(*f)(xEvent.xbutton.x, xEvent.xbutton.y);
            }
         }
         break;
         case MotionNotify: {
            callbackMap_t::iterator it = m_callbacks.find(EVENT_MOUSEMOVE);
            if (it != m_callbacks.end()) {
              for (callbackList_t::iterator f = it->second.begin(); f != it->second.end(); ++f)
                boost::get<Functor<void, TYPELIST_2(int, int)> >(*f)(xEvent.xmotion.x, xEvent.xmotion.y);
            }
         }
         break;
         case ClientMessage: {
            callbackMap_t::iterator it = m_callbacks.find(EVENT_WINCLOSE);
            if (it != m_callbacks.end()) {
              for (callbackList_t::iterator f = it->second.begin(); f != it->second.end(); ++f)
                boost::get<Functor<void, TYPELIST_0()> >(*f)();
            }
         }
         break;
         case ConfigureNotify: {
            int w = m_width;
            int h = m_height;
            if (w != xEvent.xconfigure.width || h != xEvent.xconfigure.height) {
              m_width = xEvent.xconfigure.width;
              m_height = xEvent.xconfigure.height;
              callbackMap_t::iterator it = m_callbacks.find(EVENT_WINRESIZE);

              if (it != m_callbacks.end()) {
                for (callbackList_t::iterator f = it->second.begin(); f != it->second.end(); ++f)
                  boost::get<Functor<void, TYPELIST_2(int, int)> >(*f)(xEvent.xconfigure.width, xEvent.xconfigure.height);
              }
            }
         }
         break;
       }
     }
   }
   catch (boost::bad_get& e) {
     Exception ex("Error processing window events; bad callback function; ", __FILE__, __LINE__);
     ex.append(e.what());
     throw ex;
   }
   catch (bad_alloc& e) {
     Exception ex("Error processing window events; ", __FILE__, __LINE__);
     ex.append(e.what());
     throw ex;
   }
}

//===========================================
// WinIO::destroyWindow
//===========================================
void WinIO::destroyWindow() {
   if (!m_init) return;

   XDestroyWindow(m_display, m_win);
   XFreeColormap(m_display, m_colorMap);
   XFree(m_pVisual);
   XCloseDisplay(m_display);

   EGL_CHECK(eglMakeCurrent(m_eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT));
   EGL_CHECK(eglDestroyContext(m_eglDisplay, m_eglContext));
   EGL_CHECK(eglDestroySurface(m_eglDisplay, m_eglSurface));
   EGL_CHECK(eglTerminate(m_eglDisplay));
}


}
