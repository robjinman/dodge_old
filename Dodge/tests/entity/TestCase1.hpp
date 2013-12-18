#ifndef __TEST_CASE_1_HPP__
#define __TEST_CASE_1_HPP__


#include <vector>
#include <dodge/dodge.hpp>
#include "TestCase.hpp"


class TestCase1 : public TestCase {
   public:
      virtual void setup();
      virtual void update();
      virtual void terminate();

      virtual void onKeyDown(int key);

      virtual ~TestCase1();

   private:
      std::vector<Dodge::pEntity_t> m_entities;
      int m_state;
};


#endif
