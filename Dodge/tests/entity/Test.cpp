#include <iostream>
#include "Test.hpp"
#include "TestCase0.hpp"
#include "TestCase1.hpp"
#include "TestCase2.hpp"
#include "TestCase3.hpp"
#include "TestCase4.hpp"
#include "TestCase5.hpp"


using namespace std;
using namespace Dodge;


void Test::quit() {
   exit(EXIT_SUCCESS);
}

void Test::onKeyDown(int key) {
   m_tests[m_active]->onKeyDown(key);

   switch (key) {
      case WinIO::KEY_ESCAPE:
         quit();
      break;
      case WinIO::KEY_LEFT:
         if (m_active > 0) {
            m_tests[m_active]->terminate();
            --m_active;
            m_tests[m_active]->setup();
         }
      break;
      case WinIO::KEY_RIGHT:
         if (m_active + 1 < m_tests.size()) {
            m_tests[m_active]->terminate();
            ++m_active;
            m_tests[m_active]->setup();
         }
      break;
   }
}

void Test::onKeyUp(int key) {
   m_tests[m_active]->onKeyUp(key);
}

void Test::onLeftClick(int x, int y) {
   m_tests[m_active]->onLeftClick(x, y);
}

void Test::onRightClick(int x, int y) {
   m_tests[m_active]->onRightClick(x, y);
}

void Test::onWindowResize(int w, int h) {
   Renderer& renderer = Renderer::getInstance();

   renderer.onWindowResize(w, h);
   renderer.getCamera().setProjection(static_cast<float32_t>(w) / static_cast<float32_t>(h), 1.f);
}

void Test::run() {
   projectSettings_t settings;
   settings.targetFrameRate = 120;
   settings.globalStackSize = 102400;
   settings.workingDir = ".";

   gInitialise(settings);

   EventManager eventManager;

   WinIO win;
   win.init("Dodge Engine Test: Entity", 600, 480);
   win.createGLContext();

   win.registerCallback(WinIO::EVENT_WINCLOSE, Functor<void, TYPELIST_0()>(this, &Test::quit));
   win.registerCallback(WinIO::EVENT_KEYDOWN, Functor<void, TYPELIST_1(int)>(this, &Test::onKeyDown));
   win.registerCallback(WinIO::EVENT_KEYUP, Functor<void, TYPELIST_1(int)>(this, &Test::onKeyUp));
   win.registerCallback(WinIO::EVENT_BTN1PRESS, Functor<void, TYPELIST_2(int, int)>(this, &Test::onLeftClick));
   win.registerCallback(WinIO::EVENT_BTN3PRESS, Functor<void, TYPELIST_2(int, int)>(this, &Test::onRightClick));
   win.registerCallback(WinIO::EVENT_WINRESIZE, Functor<void, TYPELIST_2(int, int)>(this, &Test::onWindowResize));

   Renderer& renderer = Renderer::getInstance();

   Functor<void, TYPELIST_0()> fMakeContext(&win, &WinIO::createGLContext);
   Functor<void, TYPELIST_0()> fSwap(&win, &WinIO::swapBuffers);

   renderer.start(fMakeContext, fSwap);

   m_tests.push_back(pTestCase_t(new TestCase0));
   m_tests.push_back(pTestCase_t(new TestCase1));
   m_tests.push_back(pTestCase_t(new TestCase2));
   m_tests.push_back(pTestCase_t(new TestCase3));
   m_tests.push_back(pTestCase_t(new TestCase4));
   m_tests.push_back(pTestCase_t(new TestCase5));

   m_active = 0;
   m_tests[m_active]->setup();

   while (1) {
      LOOP_START

      win.doEvents();
      eventManager.doEvents();

      m_tests[m_active]->update();

      renderer.tick(Colour(0.4, 0.4, 0.4, 1.0));

      LOOP_END
   }
}
