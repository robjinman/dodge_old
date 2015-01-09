#ifndef __TEST_CASE_4_HPP__
#define __TEST_CASE_4_HPP__


#include <vector>
#include <dodge/dodge.hpp>
#include "TestCase.hpp"


class TestCase4 : public TestCase {
   public:
      virtual void setup();
      virtual void update();
      virtual void terminate();

      virtual ~TestCase4();

   private:
      std::vector<Dodge::pEntity_t> m_entities;
};


#endif
