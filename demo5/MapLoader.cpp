/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include "MapLoader.hpp"


using namespace std;
using namespace Dodge;


//===========================================
// MapLoader::loadAssets
//
// Loads assets from <assets> tag.
//===========================================
void MapLoader::loadAssets(const XmlNode data, mapSegment_t* segment) {
   try {
      XML_NODE_CHECK(data, assets);

      XmlNode node = data.firstChild();
      while (!node.isNull() && node.name() == "asset") {
         XmlAttribute attr = node.firstAttribute();
         XML_ATTR_CHECK(attr, id);
         long id = attr.getLong();

         // If asset is already loaded
         if (m_assetManager.getAssetPointer(id)) continue;

         boost::shared_ptr<Asset> asset = m_factoryFunc(node.firstChild());

         m_assetManager.addAsset(id, asset);
         if (segment) segment->assetIds.push_back(id);
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
// MapLoader::parseAssetsFile_r
//===========================================
void MapLoader::parseAssetsFile_r(const string& path, mapSegment_t* segment) {
   try {
      XmlDocument doc;
      XmlNode decl = doc.parse(path);

      if (decl.isNull())
         throw XmlException("Expected XML declaration", __FILE__, __LINE__);

      XmlNode node = decl.nextSibling();
      XML_NODE_CHECK(node, ASSETFILE);

      node = node.firstChild();
      if (node.isNull())
         throw XmlException("Expected 'using' or 'assets' tag", __FILE__, __LINE__);

      if (node.name() == "using") {

         XmlNode node_ = node.firstChild();
         while (!node_.isNull() && node_.name() == "file") {
            parseAssetsFile_r(node_.getString(), segment);
            node_ = node_.nextSibling();
         }

         node = node.nextSibling();
      }

      XML_NODE_CHECK(node, assets);
      loadAssets(node, segment);
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
      XML_NODE_CHECK(node, boundary);
      m_mapBoundary = Range(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, numSegments);
      Vec2i nSegs(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, segmentSize);
      m_segmentSize = Vec2f(node.firstChild());

      node = node.nextSibling();
      XML_NODE_CHECK(node, segmentsDir);
      string dir = node.getString();

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
// MapLoader::parseMapFile
//===========================================
void MapLoader::parseMapFile(const std::string& file) {
   m_segments.clear();
   m_centreSegment = Vec2i(-1, -1);

   try {
      XmlDocument doc;

      XmlNode decl = doc.parse(file);
      if (decl.isNull())
         throw XmlException("Expected XML declaration", __FILE__, __LINE__);

      XmlNode node = decl.nextSibling();
      XML_NODE_CHECK(node, MAPFILE);

      node = node.firstChild();
      if (!node.isNull() && node.name() == "customSettings") {
         m_setMapSettingsFunc(node);
         node = node.nextSibling();
      }

      loadMapSettings(node);

      node = node.nextSibling();
      if (!node.isNull() && node.name() == "using") {

         XmlNode node_ = node.firstChild();
         while (!node_.isNull() && node_.name() == "file") {
            parseAssetsFile_r(node_.getString(), NULL);
            node_ = node_.nextSibling();
         }

         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "assets")
         loadAssets(node, NULL);
   }
   catch (XmlException& e) {
      e.prepend("Error loading map; ");
      throw;
   }
}

//===========================================
// MapLoader::unloadRefCountZeroAssets
//===========================================
void MapLoader::unloadRefCountZeroAssets() {
   for (auto i = m_refCounts.begin(); i != m_refCounts.end(); ++i) {
      if (i->second == 0) {
         pAsset_t asset = m_assetManager.getAssetPointer(i->first);

         if (asset) {
            m_deleteAssetFunc(asset);
            m_assetManager.freeAsset(i->first);
         }
      }
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

   parseAssetsFile_r(m_segments[indices.x][indices.y].filePath, &m_segments[indices.x][indices.y]);
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

   const Vec2f& mapPos = m_mapBoundary.getPosition();
   Vec2f pos(mapPos.x + static_cast<float32_t>(indices.x) * m_segmentSize.x,
             mapPos.y + static_cast<float32_t>(indices.y) * m_segmentSize.y);

   range->setPosition(pos);
   range->setSize(m_segmentSize);
}

//===========================================
// MapLoader::getSegment
//
// Returns (-1, -1) if pos is not inside any segment.
//===========================================
Vec2i MapLoader::getSegment(const Vec2f& pos) const {
   Vec2f tmp = pos - m_mapBoundary.getPosition();
   Vec2i indices(floor(tmp.x / m_segmentSize.x), floor(tmp.y / m_segmentSize.y));

   if (indices.x < 0 || indices.x > static_cast<int>(m_segments.size())
      || indices.y < 0 || indices.y > static_cast<int>(m_segments[indices.x].size())) {

      return Vec2i(-1, -1);
   }

   return indices;
}
