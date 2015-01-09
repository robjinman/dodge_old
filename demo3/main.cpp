#include <iostream>
#include "Game.hpp"


int main() {
   try {
      Game game;
      game.launch();
   }
   catch (Dodge::Exception& e) {
      std::cerr << "An error occured: " << e.what() << "\n";
      return -1;
   }

   return 0;
}
