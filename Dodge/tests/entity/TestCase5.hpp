#ifndef __TEST_CASE_5_HPP__
#define __TEST_CASE_5_HPP__


#include <vector>
#include <dodge/dodge.hpp>
#include "TestCase.hpp"


class TestCase5 : public TestCase {
   public:
      virtual void setup();
      virtual void update();
      virtual void terminate();
      virtual void onKeyDown(int key);

      virtual ~TestCase5();

   private:
      void onEvent(const Dodge::EEvent* event);
      void setupState1();
      void setupState2();
      void setupState3();
      void setupState4();
      void setupState5();
      void setupState6();
      void setupState7();
      void setupState8();

      Dodge::EventManager m_eventManager;
      std::vector<Dodge::pEntity_t> m_entities;
      int m_state;
};


#endif
