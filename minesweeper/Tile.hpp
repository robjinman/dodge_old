#ifndef __TILE_HPP__
#define __TILE_HPP__


#include <dodge/ui/UiButton.hpp>
#include <dodge/xml/xml.hpp>
#include <dodge/StringId.hpp>
#include "Item.hpp"


class Tile : public Item, public Dodge::UiButton {
   public:
      explicit Tile(const Dodge::XmlNode data)
         : Asset(Dodge::internString("Tile")),
           Entity(data.firstChild().firstChild()),
           Item(data.firstChild()),
           UiButton(data.nthChild(1)) {

         XML_NODE_CHECK(data, Tile);
      }

      Tile(const Tile& copy)
         : Asset(Dodge::internString("Tile")),
           Entity(copy),
           Item(copy),
           UiButton(copy) {}

      Tile(const Tile& copy, long name)
         : Asset(Dodge::internString("Tile")),
           Entity(copy, name),
           Item(copy, name),
           UiButton(copy, name) {}

      virtual void update();

      virtual size_t getSize() const;
      virtual Tile* clone() const;
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif
      virtual void assignData(const Dodge::XmlNode data);
};

typedef boost::shared_ptr<Tile> pTile_t;


#endif
