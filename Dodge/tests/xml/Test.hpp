#ifndef __TEST_HPP__
#define __TEST_HPP__


class Test {
   public:
      void run(bool verbose = false);

   private:
      bool testCase0() const;
      bool testCase1() const;
      bool testCase2() const;
      bool testCase3() const;
      bool testCase4() const;

      bool m_verbose;
};


#endif
