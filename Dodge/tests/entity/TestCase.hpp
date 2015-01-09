#ifndef __TEST_CASE_HPP__
#define __TEST_CASE_HPP__


#include <memory>


class TestCase {
   public:
      virtual void setup() = 0;
      virtual void update() = 0;
      virtual void terminate() = 0;

      virtual void onKeyDown(int key) {}
      virtual void onKeyUp(int key) {}
      virtual void onRightClick(int x, int y) {}
      virtual void onLeftClick(int x, int y) {}

      virtual ~TestCase() {}
};

typedef std::unique_ptr<TestCase> pTestCase_t;


#endif
