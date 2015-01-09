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
int main(int argc, char** argv) {
   try {
      Dodge::gInitialise();

      Application app;
      app.onExit(Functor<void, TYPELIST_0()>(programExit));
      app.begin(argc, argv);
   }
   catch (Dodge::Exception& e) {
      std::cerr << e.what() << "\n";
   }

   return 0;
}
