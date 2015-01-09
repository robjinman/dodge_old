#include <cstring>
#include "Test.hpp"


int main(int argc, char** argv) {
   bool verbose = false;

   for (int i = 1; i < argc; ++i) {
      if (strcmp("-v", argv[i]) == 0) verbose = true;
      if (strcmp("--verbose", argv[i]) == 0) verbose = true;
   }

   Test test;
   test.run(verbose);

   return EXIT_SUCCESS;
}
