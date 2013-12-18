#ifndef __TEST_CASE_0_HPP__
#define __TEST_CASE_0_HPP__


#include <dodge/dodge.hpp>
#include "TestCase.hpp"


class TestCase0 : public TestCase {
   public:
      virtual void setup();
      virtual void update();
      virtual void terminate();

      virtual ~TestCase0();

   private:
      Dodge::pEntity_t m_entity0;
};


#endif
