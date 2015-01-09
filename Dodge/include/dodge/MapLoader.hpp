/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __MAP_LOADER_HPP__
#define __MAP_LOADER_HPP__


#include <string>
#include <vector>
#include <list>
#include <map>
#include "../utils/Functor.hpp"
#include "xml/xml.hpp"
#include "AssetManager.hpp"
#include "Range.hpp"
#include "Exception.hpp"
#include "math/Vec2i.hpp"


namespace Dodge {


// The user of this class should make sure to not hold onto any assets that belong to a segment,
// as this would cause the asset to persist (due to boost::shared_ptr) and to later be reloaded,
// thereby duplicating it. Erasing assets from the asset manager is safe, however.
class MapLoader {
   public:
      static MapLoader& getInstance() {
         if (!m_instance) m_instance = new MapLoader;

         return *m_instance;
      }

      void initialise(Functor<void, TYPELIST_1(const XmlNode)> setMapSettingsFunc,
         Functor<pAsset_t, TYPELIST_1(const XmlNode)> factoryFunc,
         Functor<void, TYPELIST_1(pAsset_t)> deleteAssetFunc,
         size_t targetMemoryUsage);

      void parseMapFile(const std::string& directory, const std::string& name);
      void update(const Vec2f& cameraPos);
      void freeAllAssets();

      inline const Range& getMapBoundary() const;

#ifdef DEBUG
      inline size_t dbg_getMemoryUsage() const;
#endif

   private:
      MapLoader();

      static MapLoader* m_instance;

      struct mapSegment_t {
         mapSegment_t() : loaded(false) {}

         std::string filePath;
         std::vector<long> assetIds;
         bool loaded;
      };

      class refCountTable_t {
         private:
            typedef unsigned int refCount_t;
            typedef long id_t;

         public:
            inline void incrRefCount(id_t id);
            inline void decrRefCount(id_t id);
            inline refCount_t getRefCount(id_t id) const;
            inline void clear();

         private:
            void insert(id_t id, refCount_t refCount);
            void erase(id_t id);

            std::map<id_t, refCount_t> m_container;
      };

      bool m_init;

      std::string m_dataRoot;

      refCountTable_t m_refCountTable;

      // Segments at the front of the list have been pending the longest
      std::list<Vec2i> m_pendingUnload;

      AssetManager m_assetManager;

      Functor<void, TYPELIST_1(const XmlNode)> m_setMapSettingsFunc;
      Functor<pAsset_t, TYPELIST_1(const XmlNode)> m_factoryFunc;
      Functor<void, TYPELIST_1(pAsset_t)> m_deleteAssetFunc;

      Range m_mapBoundary;
      std::vector<std::vector<mapSegment_t> > m_segments;
      Vec2i m_centreSegment;
      Vec2f m_segmentSize;

      size_t m_targetMemUsage;
      size_t m_currentMemUsage;

      Vec2i getSegment(const Vec2f& pos) const;
      void setPendingUnload(const Vec2i& indices, bool b);
      void loadMapSettings(const XmlNode data);
      void loadSegment(const Vec2i& indices);
      void unloadSegments();
      void loadAssets(const XmlNode data, mapSegment_t* segment);
      void parseAssetsFile_r(const std::string& path, mapSegment_t* segment);

      size_t getMemoryUsage() const;
};

//===========================================
// MapLoader::refCountTable_t::clear
//===========================================
inline void MapLoader::refCountTable_t::clear() {
   m_container.clear();
}

//===========================================
// MapLoader::refCountTable_t::erase
//===========================================
inline void MapLoader::refCountTable_t::erase(id_t id) {
   m_container.erase(id);
}

//===========================================
// MapLoader::refCountTable_t::insert
//===========================================
inline void MapLoader::refCountTable_t::insert(id_t id, refCount_t refCount) {
   m_container[id] = refCount;
}

//===========================================
// MapLoader::refCountTable_t::getRefCount
//===========================================
inline MapLoader::refCountTable_t::refCount_t MapLoader::refCountTable_t::getRefCount(id_t id) const {
   auto i = m_container.find(id);
   return i != m_container.end() ? i->second : -1;
}

//===========================================
// MapLoader::refCountTable_t::incrRefCount
//===========================================
inline void MapLoader::refCountTable_t::incrRefCount(id_t id) {
   auto i = m_container.find(id);

   if (i != m_container.end())
      ++i->second;
   else
      insert(id, 1);
}

//===========================================
// MapLoader::refCountTable_t::decrRefCount
//===========================================
inline void MapLoader::refCountTable_t::decrRefCount(id_t id) {
   auto i = m_container.find(id);

   if (i != m_container.end() && i->second > 0)
      --i->second;
}

#ifdef DEBUG
//===========================================
// MapLoader::dbg_getMemoryUsage
//===========================================
inline size_t MapLoader::dbg_getMemoryUsage() const {
   if (!m_init) return 0;

   return getMemoryUsage();
}
#endif

//===========================================
// MapLoader::getMapBoundary
//===========================================
inline const Range& MapLoader::getMapBoundary() const {
   if (!m_init)
      throw Exception("Error retrieving map boundary; MapLoader not initialised", __FILE__, __LINE__);

   return m_mapBoundary;
}


}


#endif
