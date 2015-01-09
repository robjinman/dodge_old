#include <string>
#include <sstream>
#include <iostream>
#include <dodge/dodge.hpp>
#include "Test.hpp"


using namespace std;
using namespace Dodge;


void Test::run(bool verbose) {
   bool pass = true;

   gInitialise();

   cout << "TEST: Polygon\n";

   Polygon poly1;
   poly1.addVertex(Vec2f(1, 1));
   poly1.addVertex(Vec2f(2, 1));
   poly1.addVertex(Vec2f(1.5, 2));

   Polygon poly2(poly1);

   Polygon poly3 = poly2;

   cout << "RESULT: " << (pass ? "PASS" : "FAIL") << "\n";
}
