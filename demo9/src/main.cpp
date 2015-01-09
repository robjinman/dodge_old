#include <iostream>
#include "Game.hpp"


using namespace std;


int main(int argc, char** argv) {
   int r = EXIT_FAILURE;

   try {
      Game game;
      r = game.launch(argc, argv);
   }
   catch (exception& e) {
      cerr << "Program received fatal error: " << e.what() << "\n";
   }

   return r;
}
