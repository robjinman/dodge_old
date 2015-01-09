#ifndef __TEST_HPP__
#define __TEST_HPP__


#include <vector>
#include <dodge/dodge.hpp>
#include "TestCase.hpp"


class Test {
   public:
      void run();

   private:
      void onKeyDown(int key);
      void onKeyUp(int key);
      void onRightClick(int x, int y);
      void onLeftClick(int x, int y);
      void onWindowResize(int w, int h);
      void quit();

      std::vector<pTestCase_t> m_tests;
      unsigned int m_active;
};


#endif
