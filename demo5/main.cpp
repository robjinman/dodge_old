#include <iostream>
#include <dodge/Exception.hpp>
#include "Application.hpp"


//===========================================
// programExit
//===========================================
void programExit() {
   exit(0);
}

//===========================================
// main
//===========================================
int main() {
   try {
      Application app;
      app.onExit(Functor<void, TYPELIST_0()>(programExit));
      app.begin();
   }
   catch (Dodge::Exception& e) {
      std::cerr << e.what() << "\n";
   }

   return 0;
}
