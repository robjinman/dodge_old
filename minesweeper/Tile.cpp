#include "Tile.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// Tile::update
//===========================================
void Tile::update() {
   UiButton::update();
}

//===========================================
// Tile::getSize
//===========================================
size_t Tile::getSize() const {
   return sizeof(Tile)
      - sizeof(Item)
      + Item::getSize()
      - sizeof(UiButton)
      + UiButton::getSize()
      - sizeof(Asset);
}

//===========================================
// Tile::clone
//===========================================
Tile* Tile::clone() const {
   return new Tile(*this);
}

#ifdef DEBUG
//===========================================
// Tile::dbg_print
//===========================================
void Tile::dbg_print(ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";

   out << "Tile\n";

   Item::dbg_print(out, tab + 1);
   UiButton::dbg_print(out, tab + 1);
}
#endif

//===========================================
// Tile::assignData
//===========================================
void Tile::assignData(const Dodge::XmlNode data) {
   if (data.isNull() || data.name() != "Tile") return;

   XmlNode node = data.firstChild();
   if (!node.isNull() && node.name() == "Item") {
      Item::assignData(node);
      node = node.nextSibling();
   }

   if (!node.isNull() && node.name() == "UiButton") {
      UiButton::assignData(node);
   }
}
