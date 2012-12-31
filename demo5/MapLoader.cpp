/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include "MapLoader.hpp"
#include "Soil.hpp"
#include "CSprite.hpp"
#include "CParallaxSprite.hpp"
#include "CPhysicalEntity.hpp"
#include "CPhysicalSprite.hpp"
#include "Player.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// MapLoader::constructItem
//===========================================
boost::shared_ptr<Item> MapLoader::constructItem(const XmlNode data) const {
   if (data.name() == "Player") return pItem_t(new Player(data));
   if (data.name() == "Soil") return pItem_t(new Soil(data));
   if (data.name() == "Item") return pItem_t(new Item(data));
   if (data.name() == "CParallaxSprite") return pItem_t(new CParallaxSprite(data));
   if (data.name() == "CSprite") return pItem_t(new CSprite(data));
   if (data.name() == "CPhysicalEntity") return pItem_t(new CPhysicalEntity(data));
   if (data.name() == "CPhysicalSprite") return pItem_t(new CPhysicalSprite(data));

   throw Exception("Unrecognised item type", __FILE__, __LINE__);
}

//===========================================
// MapLoader::constructAsset
//
// If proto = -1 asset is *not* constructed from prototype
//===========================================
boost::shared_ptr<Asset> MapLoader::constructAsset(const XmlNode data, long proto, bool addToWorld) {
   if (data.isNull())
      throw XmlException("Error constructing asset; XML node is empty", __FILE__, __LINE__);

   boost::shared_ptr<Asset> asset;

   // Construct non-Item assets
   if (data.name() == "Texture") {
      asset = boost::shared_ptr<Asset>(new Texture(data));
   }
   // Construct Items
   else {
      pItem_t item;

      if (proto == -1) {
         asset = item = constructItem(data);
      }
      else {
         asset = item = pItem_t(dynamic_cast<Item*>(m_gameMap->assetManager.cloneAsset(proto)));

         if (!item)
            throw XmlException("Error constructing asset; Bad prototype id", __FILE__, __LINE__);

         item->assignData(data);
      }

      if (addToWorld) {
         item->addToWorld();
         m_gameMap->worldSpace.insertAndTrackEntity(item);
         m_gameMap->items[item->getName()] = item;
      }
   }

   return asset;
}

//===========================================
// MapLoader::loadAssets_r
//===========================================
void MapLoader::loadAssets_r(const XmlNode data, mapSegment_t* segment, int depth) {
   try {
      if (data.isNull())
         throw XmlException("Expected XML declaration", __FILE__, __LINE__);

      XmlNode node = data.nextSibling();
      XML_NODE_CHECK(node, ASSETFILE);

      node = node.firstChild();
      if (node.isNull())
         throw XmlException("Expected 'using' or 'assets' tag", __FILE__, __LINE__);

      if (node.name() == "using") {
         XmlNode node_ = node.firstChild();
         while (!node_.isNull() && node_.name() == "file") {
            string path = string("data/xml/").append(node_.getString());

            XmlDocument doc;
            XmlNode decl = doc.parse(path);
            loadAssets_r(decl, segment, depth + 1);

            node_ = node_.nextSibling();
         }

         node = node.nextSibling();
      }

      XML_NODE_CHECK(node, assets);

      node = node.firstChild();
      while (!node.isNull() && node.name() == "asset") {
         XmlAttribute attr = node.firstAttribute();
         XML_ATTR_CHECK(attr, id);
         long id = attr.getLong();

         long proto = -1;
         attr = attr.nextAttribute();
         if (!attr.isNull() && attr.name() == "proto")
            proto = attr.getLong();

         boost::shared_ptr<Asset> asset = constructAsset(node.firstChild(), proto, depth == 0 ? true : false);

         m_gameMap->assetManager.addAsset(id, asset);
         segment->assetIds.push_back(id);
         ++m_refCounts[id];

         node = node.nextSibling();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error loading assets from XML file; ");
      throw;
   }
}

//===========================================
// MapLoader::loadMapSettings
//===========================================
void MapLoader::loadMapSettings(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, settings);

      XmlNode node = data.firstChild();
      XML_NODE_CHECK(node, bgColour);
      m_gameMap->bgColour = Colour(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, boundary);
      m_gameMap->mapBoundary = Range(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, numSegments);
      Vec2i nSegs(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, segmentSize);
      m_segmentSize = Vec2f(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, segmentsDir);
      string dir = node.getString();

      node = node.nextSibling();
      XML_NODE_CHECK(node, fillerTile);

      XmlAttribute attr = node.firstAttribute();
      XML_ATTR_CHECK(attr, id);
      m_fillerTileId = attr.getLong();

      for (int i = 0; i < nSegs.x; ++i) {
         m_segments.push_back(vector<mapSegment_t>());

         for (int j = 0; j < nSegs.y; ++j) {
            mapSegment_t seg;

            stringstream path;
            path << dir << "/" << i << j << ".xml";
            seg.filePath = path.str();

            m_segments[i].push_back(seg);
         }
      }
   }
   catch (XmlException& e) {
      e.prepend("Error loading map settings; ");
      throw;
   }
}

//===========================================
// MapLoader::fillRange
//
// Fills any empty spaces within the section of map specified by 'range' with the filler tile.
//===========================================
void MapLoader::fillRange(const Range& range) {
   static long stopBlockStr = internString("stopBlock");

   pItem_t filler = boost::dynamic_pointer_cast<Item>(m_gameMap->assetManager.getAssetPointer(m_fillerTileId));
   if (!filler) throw Exception("Bad filler tile id", __FILE__, __LINE__);

   float32_t w = filler->getBoundary().getSize().x;
   float32_t h = filler->getBoundary().getSize().y;

   unique_ptr<Shape> shape(filler->getShape().clone());
   shape->scale(Vec2f(0.9f, 0.9f));

   const Vec2f& pos = range.getPosition();
   const Vec2f& sz = range.getSize();

   for (float32_t x = pos.x; x < pos.x + sz.x; x += w) {
      for (float32_t y = pos.y; y < pos.y + sz.y; y += h) {

         vector<pEntity_t> vec;
         m_gameMap->worldSpace.getEntities(filler->getBoundary(), vec);
         bool clear = true;
         for (uint_t i = 0; i < vec.size(); ++i) {
            if (!vec[i]->hasShape()) continue;
            if (vec[i]->getTypeName() == stopBlockStr) continue;

            if (Math::overlap(*shape, Vec2f(x + 0.005, y + 0.005), vec[i]->getShape(), vec[i]->getTranslation_abs())) {
               clear = false;
               break;
            }
         }

         if (clear) {
            pItem_t item(filler->clone());

            item->setTranslation(x, y);

            item->addToWorld();
            m_gameMap->worldSpace.trackEntity(item);
            m_gameMap->items[item->getName()] = item;
         }
      }
   }
}

//===========================================
// MapLoader::parseMapFile
//===========================================
void MapLoader::parseMapFile(const std::string& file, gameMap_t* gameMap) {
   m_segments.clear();
   m_centreSegment = Vec2i(-1, -1);

   try {
      m_gameMap = gameMap;

      XmlDocument doc;

      XmlNode decl = doc.parse(file);
      if (decl.isNull())
         throw XmlException("Expected XML declaration", __FILE__, __LINE__);

      XmlNode node = decl.nextSibling();
      XML_NODE_CHECK(node, MAPFILE);

      node = node.firstChild();
      loadMapSettings(node);
   }
   catch (XmlException& e) {
      e.prepend("Error loading map; ");
      throw;
   }
}

//===========================================
// MapLoader::parseMapSegment
//===========================================
void MapLoader::parseMapSegment(mapSegment_t& segment) {
   try {
      segment.data = segment.xmlDocument->parse(segment.filePath);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing map segment; ");
      throw;
   }
}

//===========================================
// MapLoader::unloadRefCountZeroAssets
//===========================================
void MapLoader::unloadRefCountZeroAssets() {
   // TODO: This currently only removes the asset from the asset manager.
   // It must also be removed from all other containers.
   // Some sort of message system should be used to communicate to other parts of
   // the program that the asset should be deleted. This message system must be
   // instantaneous.
   //
   // Once this is implemented, the gameMap->items, and m_gameMap->worldspace containers
   // can reside elsewhere, helping to make this class less game-specific, and more
   // generic -- ultimately enabling it to be factored out into the engine.

   for (auto i = m_refCounts.begin(); i != m_refCounts.end(); ++i) {
      if (i->second == 0) m_gameMap->assetManager.freeAsset(i->first);
   }
}

//===========================================
// MapLoader::unloadSegment
//
// Decrement ref. counts for assets used by segment
//===========================================
void MapLoader::unloadSegment(const Vec2i& indices) {
   if (indices.x < 0 || indices.x >= static_cast<int>(m_segments.size())
      || indices.y < 0 || indices.y >= static_cast<int>(m_segments[indices.x].size())) return;

   mapSegment_t& seg = m_segments[indices.x][indices.y];

   for (uint_t i = 0; i < seg.assetIds.size(); ++i) {
      unsigned int c = m_refCounts[seg.assetIds[i]];
      if (c > 0) m_refCounts[seg.assetIds[i]] = c - 1;
   }
}

//===========================================
// MapLoader::loadSegment
//===========================================
void MapLoader::loadSegment(const Vec2i& indices) {
   if (indices.x < 0 || indices.x >= static_cast<int>(m_segments.size())
      || indices.y < 0 || indices.y >= static_cast<int>(m_segments[indices.x].size())) return;

   if (m_segments[indices.x][indices.y].data.isNull())
      parseMapSegment(m_segments[indices.x][indices.y]);

   loadAssets_r(m_segments[indices.x][indices.y].data, &m_segments[indices.x][indices.y]);

   Range range;
   getSegmentRange(indices, &range);

   fillRange(range);
}

//===========================================
// MapLoader::update
//===========================================
void MapLoader::update(const Vec2f& viewPos) {
   Vec2i current = getSegment(viewPos);

   if (current == Vec2i(-1, -1)) return;

   // If map segments are not loaded
   if (m_centreSegment == Vec2i(-1, -1)) {
      loadSegment(current + Vec2i(-1, -1));
      loadSegment(current + Vec2i(-1, 0));
      loadSegment(current + Vec2i(-1, 1));

      loadSegment(current + Vec2i(0, -1));
      loadSegment(current + Vec2i(0, 0));
      loadSegment(current + Vec2i(0, 1));

      loadSegment(current + Vec2i(1, -1));
      loadSegment(current + Vec2i(1, 0));
      loadSegment(current + Vec2i(1, 1));
   }
   else {
      // If camera has moved
      if (current != m_centreSegment) {

         set<pair<int, int> > prvSegs;

         prvSegs.insert(pair<int, int>(m_centreSegment.x - 1, m_centreSegment.y - 1));
         prvSegs.insert(pair<int, int>(m_centreSegment.x - 1, m_centreSegment.y + 0));
         prvSegs.insert(pair<int, int>(m_centreSegment.x - 1, m_centreSegment.y + 1));

         prvSegs.insert(pair<int, int>(m_centreSegment.x + 0, m_centreSegment.y - 1));
         prvSegs.insert(pair<int, int>(m_centreSegment.x + 0, m_centreSegment.y + 0));
         prvSegs.insert(pair<int, int>(m_centreSegment.x + 0, m_centreSegment.y + 1));

         prvSegs.insert(pair<int, int>(m_centreSegment.x + 1, m_centreSegment.y - 1));
         prvSegs.insert(pair<int, int>(m_centreSegment.x + 1, m_centreSegment.y + 0));
         prvSegs.insert(pair<int, int>(m_centreSegment.x + 1, m_centreSegment.y + 1));

         set<pair<int, int> > curSegs;

         curSegs.insert(pair<int, int>(current.x - 1, current.y - 1));
         curSegs.insert(pair<int, int>(current.x - 1, current.y + 0));
         curSegs.insert(pair<int, int>(current.x - 1, current.y + 1));

         curSegs.insert(pair<int, int>(current.x + 0, current.y - 1));
         curSegs.insert(pair<int, int>(current.x + 0, current.y + 0));
         curSegs.insert(pair<int, int>(current.x + 0, current.y + 1));

         curSegs.insert(pair<int, int>(current.x + 1, current.y - 1));
         curSegs.insert(pair<int, int>(current.x + 1, current.y + 0));
         curSegs.insert(pair<int, int>(current.x + 1, current.y + 1));

         for (auto i = prvSegs.begin(); i != prvSegs.end(); ++i) {
            if (curSegs.find(*i) == curSegs.end())
               unloadSegment(Vec2i(i->first, i->second));
         }

         for (auto i = curSegs.begin(); i != curSegs.end(); ++i) {
            if (prvSegs.find(*i) == prvSegs.end())
               loadSegment(Vec2i(i->first, i->second));
         }

         unloadRefCountZeroAssets();
      }
   }

   m_centreSegment = current;
}

//===========================================
// MapLoader::getSegmentRange
//
// Returns the range spanned by the map segment at position 'indices' in the m_segments array.
//===========================================
void MapLoader::getSegmentRange(const Vec2i& indices, Range* range) const {
   if (indices.x < 0 || indices.x >= static_cast<int>(m_segments.size())
      || indices.y < 0 || indices.y >= static_cast<int>(m_segments[indices.x].size())) {

      throw Exception("Cannot retrieve map segment; Index out of range", __FILE__, __LINE__);
   }

   const Vec2f& mapPos = m_gameMap->mapBoundary.getPosition();
   Vec2f pos(mapPos.x + static_cast<float32_t>(indices.x) * m_segmentSize.x,
             mapPos.x + static_cast<float32_t>(indices.x) * m_segmentSize.x);

   range->setPosition(pos);
   range->setSize(m_segmentSize);
}

//===========================================
// MapLoader::getSegment
//
// Returns (-1, -1) if pos is not inside any segment.
//===========================================
Vec2i MapLoader::getSegment(const Vec2f& pos) const {
   Vec2f tmp = pos - m_gameMap->mapBoundary.getPosition();
   Vec2i indices(floor(tmp.x / m_segmentSize.x), floor(tmp.y / m_segmentSize.y));

   if (indices.x < 0 || indices.x > static_cast<int>(m_segments.size())
      || indices.y < 0 || indices.y > static_cast<int>(m_segments[indices.x].size())) {

      return Vec2i(-1, -1);
   }

   return indices;
}
