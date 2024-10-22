/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#include <set>
#include <sstream>
#include <globals.hpp>
#include <MapLoader.hpp>


using namespace std;


namespace Dodge {


MapLoader* MapLoader::m_instance = NULL;


void dummyFunc1(const XmlNode) {}
pAsset_t dummyFunc2(const XmlNode) { return pAsset_t(); }
void dummyFunc3(pAsset_t) {}


//===========================================
// MapLoader::MapLoader
//===========================================
MapLoader::MapLoader()
   : m_init(false),
     m_setMapSettingsFunc(dummyFunc1),
     m_factoryFunc(dummyFunc2),
     m_deleteAssetFunc(dummyFunc3),
     m_currentMemUsage(0) {}

//===========================================
// MapLoader::initialise
//===========================================
void MapLoader::initialise(Functor<void, TYPELIST_1(const Dodge::XmlNode)> setMapSettingsFunc,
   Functor<Dodge::pAsset_t, TYPELIST_1(const Dodge::XmlNode)> factoryFunc,
   Functor<void, TYPELIST_1(Dodge::pAsset_t)> deleteAssetFunc,
   size_t targetMemoryUsage) {

   m_setMapSettingsFunc = setMapSettingsFunc;
   m_factoryFunc = factoryFunc;
   m_deleteAssetFunc = deleteAssetFunc;
   m_targetMemUsage = targetMemoryUsage;

   m_init = true;
}

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
         XML_ATTR_CHECK(attr, assetId);
         long id = attr.getLong();

         // If asset is not already loaded
         if (!m_assetManager.getAssetPointer(id)) {
            boost::shared_ptr<Asset> asset = m_factoryFunc(node);
            m_assetManager.addAsset(id, asset);
            m_currentMemUsage += asset->getSize();
         }

         m_refCountTable.incrRefCount(id);

         if (segment) segment->assetIds.push_back(id);

         node = node.nextSibling();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error loading assets from XML file; ");
      throw;
   }
}

//===========================================
// MapLoader::getMemoryUsage
//===========================================
size_t MapLoader::getMemoryUsage() const {

   size_t total = 0;
   for (auto i = m_assetManager.begin(); i != m_assetManager.end(); ++i)
      total += i->second->getSize();

   return total;
}

//===========================================
// MapLoader::parseAssetsFile_r
//===========================================
void MapLoader::parseAssetsFile_r(const string& path, mapSegment_t* segment) {
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
         stringstream ss;
         ss << m_dataRoot << "/" << node_.getString();

         parseAssetsFile_r(ss.str(), segment);
         node_ = node_.nextSibling();
      }

      node = node.nextSibling();
   }

   XML_NODE_CHECK(node, assets);
   loadAssets(node, segment);
   node = node.nextSibling();

   if (!node.isNull() && node.name() == "using") {

      XmlNode node_ = node.firstChild();
      while (!node_.isNull() && node_.name() == "file") {
         stringstream ss;
         ss << m_dataRoot << "/" << node_.getString();

         parseAssetsFile_r(ss.str(), segment);
         node_ = node_.nextSibling();
      }
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
      XML_NODE_CHECK(node, segmentsPath);
      string dir = node.getString();

      for (int i = 0; i < nSegs.x; ++i) {
         m_segments.push_back(vector<mapSegment_t>());

         for (int j = 0; j < nSegs.y; ++j) {
            mapSegment_t seg;

            stringstream path;
            path << m_dataRoot << "/" << dir << "/" << i << j << ".xml";
            seg.filePath = path.str();

            m_segments[i].push_back(seg);
         }
      }

      node = node.nextSibling();
      XML_NODE_CHECK(node, customSettings);
      m_setMapSettingsFunc(node);
   }
   catch (XmlException& e) {
      e.prepend("Error loading map settings; ");
      throw;
   }
}

//===========================================
// MapLoader::parseMapFile
//===========================================
void MapLoader::parseMapFile(const string& directory, const string& name) {
   if (!m_init)
      throw Dodge::Exception("Error parsing map file; MapLoader not initialised", __FILE__, __LINE__);

   m_dataRoot = directory;

   m_segments.clear();
   m_centreSegment = Vec2i(-1, -1);

   try {
      XmlDocument doc;

      XmlNode decl = doc.parse(m_dataRoot + "/" + name);
      if (decl.isNull())
         throw XmlException("Expected XML declaration", __FILE__, __LINE__);

      XmlNode node = decl.nextSibling();
      XML_NODE_CHECK(node, MAPFILE);

      node = node.firstChild();
      loadMapSettings(node);
      node = node.nextSibling();

      if (!node.isNull() && node.name() == "using") {

         XmlNode node_ = node.firstChild();
         while (!node_.isNull() && node_.name() == "file") {
            stringstream ss;
            ss << m_dataRoot << "/" << node_.getString();

            parseAssetsFile_r(ss.str(), NULL);
            node_ = node_.nextSibling();
         }

         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "assets") {
         loadAssets(node, NULL);
         node = node.nextSibling();
      }

      if (!node.isNull() && node.name() == "using") {

         XmlNode node_ = node.firstChild();
         while (!node_.isNull() && node_.name() == "file") {
            stringstream ss;
            ss << m_dataRoot << "/" << node_.getString();

            parseAssetsFile_r(ss.str(), NULL);
            node_ = node_.nextSibling();
         }
      }
   }
   catch (XmlException& e) {
      e.prepend("Error loading map; ");
      throw;
   }
}

//===========================================
// MapLoader::unloadSegments
//===========================================
void MapLoader::unloadSegments() {
   while (m_currentMemUsage > m_targetMemUsage) {
      auto i = m_pendingUnload.begin();

      if (i != m_pendingUnload.end()) {
         mapSegment_t& seg = m_segments[i->x][i->y];

         for (uint_t a = 0; a < seg.assetIds.size(); ++a) {
            long id = seg.assetIds[a];

            m_refCountTable.decrRefCount(id);

            if (id != -1 && m_refCountTable.getRefCount(id) == 0) {
               pAsset_t asset = m_assetManager.getAssetPointer(id);

               if (asset) {
                  m_currentMemUsage -= asset->getSize();
                  m_deleteAssetFunc(asset);
                  m_assetManager.freeAsset(id);
               }
            }
         }

         seg.loaded = false;
         m_pendingUnload.pop_front();
      }
      else
         break;
   }
}

//===========================================
// MapLoader::loadSegment
//===========================================
void MapLoader::loadSegment(const Vec2i& indices) {
   if (indices.x < 0 || indices.x >= static_cast<int>(m_segments.size())
      || indices.y < 0 || indices.y >= static_cast<int>(m_segments[indices.x].size())) return;

   setPendingUnload(indices, false);

   mapSegment_t& seg = m_segments[indices.x][indices.y];

   if (!seg.loaded) {
      seg.assetIds.clear();
      parseAssetsFile_r(seg.filePath, &seg);
      seg.loaded = true;
   }
}

//===========================================
// MapLoader::setPendingUnload
//===========================================
void MapLoader::setPendingUnload(const Vec2i& indices, bool b) {
   if (indices.x < 0 || indices.x >= static_cast<int>(m_segments.size())
      || indices.y < 0 || indices.y >= static_cast<int>(m_segments[indices.x].size())) return;

   for (auto i = m_pendingUnload.begin(); i != m_pendingUnload.end(); ++i) {
      if (*i == indices) {
         m_pendingUnload.erase(i);
         break;
      }
   }

   if (b) m_pendingUnload.push_back(indices);
}

//===========================================
// MapLoader::update
//===========================================
void MapLoader::update(const Vec2f& viewPos) {
   if (!m_init)
      throw Dodge::Exception("Error in MapLoader::update(); MapLoader not initialised", __FILE__, __LINE__);

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
      // If camera has moved into a new segment
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
               setPendingUnload(Vec2i(i->first, i->second), true);
         }

         for (auto i = curSegs.begin(); i != curSegs.end(); ++i) {
            if (prvSegs.find(*i) == prvSegs.end())
               loadSegment(Vec2i(i->first, i->second));
         }

         unloadSegments();
      }
   }

   m_centreSegment = current;
}

//===========================================
// MapLoader::getSegment
//
// Returns (-1, -1) if pos is not inside any segment.
//===========================================
Vec2i MapLoader::getSegment(const Vec2f& pos) const {
   Vec2f tmp = pos - m_mapBoundary.getPosition();
   Vec2i indices(static_cast<int>(floor(tmp.x / m_segmentSize.x)), static_cast<int>(floor(tmp.y / m_segmentSize.y)));

   if (indices.x < 0 || indices.x > static_cast<int>(m_segments.size())
      || indices.y < 0 || indices.y > static_cast<int>(m_segments[indices.x].size())) {

      return Vec2i(-1, -1);
   }

   return indices;
}

//===========================================
// MapLoader::freeAllAssets
//===========================================
void MapLoader::freeAllAssets() {
   if (m_init) {
      m_assetManager.freeAllAssets();
      m_currentMemUsage = 0;
      m_pendingUnload.clear();
   }   
}


}
