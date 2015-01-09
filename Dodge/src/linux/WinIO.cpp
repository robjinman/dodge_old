/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <map>
#include <cstdlib>
#include <string>
#include <X11/XKBlib.h>
#include <GLEW/glew.h>
#include <WinIO.hpp>
#include <Exception.hpp>


using namespace std;


namespace Dodge {


Display* WinIO::m_display = NULL;
Window WinIO::m_root;
XVisualInfo* WinIO::m_visual = NULL;
Colormap WinIO::m_colourMap;
XSetWindowAttributes WinIO::m_setWinAttr;
Window WinIO::m_win;
GLXContext WinIO::m_context;
XWindowAttributes WinIO::m_winAttr;

WinIO::callbackMap_t WinIO::m_callbacks;
int WinIO::m_width = 0;
int WinIO::m_height = 0;

bool WinIO::m_init = false;
#ifdef DEBUG
byte_t WinIO::dbg_flags = 0;
#endif


//===========================================
// WinIO::init
//
// Sets up a new OpenGL window.
//===========================================
void WinIO::init(const std::string& winTitle, int w, int h, bool fullscreen) {
   if (fullscreen) throw Exception("Fullscreen mode not supported", __FILE__, __LINE__);

#ifdef DEBUG
   if (dbg_flags & DBG_NO_VSYNC) {
      char str[] = "vblank_mode=0";
      putenv(str);
   }
#endif

   XInitThreads();

   m_width = w;
   m_height = h;

   m_display = XOpenDisplay(NULL);
   if (m_display == NULL)
      throw Exception("Cannot connect to X server", __FILE__, __LINE__);

   m_root = DefaultRootWindow(m_display);

   GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
   m_visual = glXChooseVisual(m_display, 0, att);
   if (m_visual == NULL)
      throw Exception("No appropriate visual found", __FILE__, __LINE__);

   m_colourMap = XCreateColormap(m_display, m_root, m_visual->visual, AllocNone);
   m_setWinAttr.colormap = m_colourMap;
   m_setWinAttr.event_mask = KeyPressMask | KeyReleaseMask | ButtonPressMask
                           | ButtonReleaseMask | PointerMotionMask | Button1MotionMask
                           | Button2MotionMask | ExposureMask | StructureNotifyMask;

   m_win = XCreateWindow(m_display, m_root, 0, 0, w, h, 0, m_visual->depth, InputOutput,
      m_visual->visual, CWColormap | CWEventMask, &m_setWinAttr);

   Atom wmDelete = XInternAtom(m_display, "WM_DELETE_WINDOW", True);
   XSetWMProtocols(m_display, m_win, &wmDelete, 1);

   XSizeHints sh;
   sh.flags = USPosition;
   sh.x = 10;
   sh.y = 10;
   XSetStandardProperties(m_display, m_win, winTitle.data(), winTitle.data(), None, 0, 0, &sh);

   XMapWindow(m_display, m_win);

   // Halt until window is "mapped".
   XEvent e;
   XIfEvent(m_display, &e, waitForMap, reinterpret_cast<XPointer>(&m_win));

#ifdef DEBUG
   if (dbg_flags & DBG_NO_VSYNC)
      XSynchronize(m_display, False);
   else
      XSynchronize(m_display, True);
#else
   XSynchronize(m_display, True);
#endif

   XFlush(m_display);

   m_init = true;
}

//===========================================
// WinIO::createGLContext
//===========================================
void WinIO::createGLContext() {
   m_context = glXCreateContext(m_display, m_visual, NULL, GL_TRUE);
   glXMakeCurrent(m_display, m_win, m_context);

   if (glewInit() != GLEW_OK) {
      destroyWindow();
      throw Exception("Error creating GL context", __FILE__, __LINE__);
   }
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
// WinIO::swapBuffers
//===========================================
void WinIO::swapBuffers() {
   glXSwapBuffers(m_display, m_win);
}

//===========================================
// WinIO::doEvents
//
// Listens for GUI events, and calls the relevent
// callback functions.
//===========================================
void WinIO::doEvents() {
   if (!m_init)
      throw Exception("Error processing window events; WinIO not initialised", __FILE__, __LINE__);

   try {
      while (XPending(m_display)) {
         XEvent xEvent;
         XNextEvent(m_display, &xEvent);

         switch (xEvent.type) {
            case Expose: {
               callbackMap_t::iterator it = m_callbacks.find(EVENT_WINEXPOSE);
               if (it != m_callbacks.end()) {
               for (uint_t f = 0; f < it->second.size(); ++f)
                  boost::get<Functor<void, TYPELIST_0()> >(it->second[f])();
               }
            }
            break;
            case KeyPress: {
               callbackMap_t::iterator it = m_callbacks.find(EVENT_KEYDOWN);
               if (it != m_callbacks.end()) {
                  for (uint_t f = 0; f < it->second.size(); ++f)
                     boost::get<Functor<void, TYPELIST_1(int)> >(it->second[f])(XkbKeycodeToKeysym(m_display, xEvent.xkey.keycode, 0, 0));
               }
            }
            break;
            case KeyRelease: {
               // If next event is KeyPress of the same key, then this event is likely caused by autorepeat and should be ignored.
               if (XEventsQueued(m_display, QueuedAfterReading)) {
                  XEvent next;
                  XPeekEvent(m_display, &next);

                  if (next.type == KeyPress && next.xkey.keycode == xEvent.xkey.keycode)
                     break;
               }

               callbackMap_t::iterator it = m_callbacks.find(EVENT_KEYUP);
               if (it != m_callbacks.end()) {
                  for (uint_t f = 0; f < it->second.size(); ++f)
                     boost::get<Functor<void, TYPELIST_1(int)> >(it->second[f])(XkbKeycodeToKeysym(m_display, xEvent.xkey.keycode, 0, 0));
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
                  for (uint_t f = 0; f < it->second.size(); ++f)
                     boost::get<Functor<void, TYPELIST_2(int, int)> >(it->second[f])(xEvent.xbutton.x, xEvent.xbutton.y);
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
                  for (uint_t f = 0; f < it->second.size(); ++f)
                     boost::get<Functor<void, TYPELIST_2(int, int)> >(it->second[f])(xEvent.xbutton.x, xEvent.xbutton.y);
               }
            }
            break;
            case MotionNotify: {
               callbackMap_t::iterator it = m_callbacks.find(EVENT_MOUSEMOVE);
               if (it != m_callbacks.end()) {
                  for (uint_t f = 0; f < it->second.size(); ++f)
                     boost::get<Functor<void, TYPELIST_2(int, int)> >(it->second[f])(xEvent.xmotion.x, xEvent.xmotion.y);
               }
            }
            break;
            case ClientMessage: {
               callbackMap_t::iterator it = m_callbacks.find(EVENT_WINCLOSE);
               if (it != m_callbacks.end()) {
                  for (uint_t f = 0; f < it->second.size(); ++f)
                     boost::get<Functor<void, TYPELIST_0()> >(it->second[f])();
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
                     for (uint_t f = 0; f < it->second.size(); ++f)
                        boost::get<Functor<void, TYPELIST_2(int, int)> >(it->second[f])(xEvent.xconfigure.width, xEvent.xconfigure.height);
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

   glXMakeCurrent(m_display, None, NULL);
   glXDestroyContext(m_display, m_context);
   XDestroyWindow(m_display, m_win);
   XCloseDisplay(m_display);
}


}
