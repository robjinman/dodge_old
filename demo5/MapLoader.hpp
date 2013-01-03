/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __MAP_LOADER_HPP__
#define __MAP_LOADER_HPP__


#include <dodge/dodge.hpp>
#include <string>
#include <vector>
#include <map>
#include <boost/shared_ptr.hpp>


class MapLoader {
   public:
      MapLoader(Dodge::AssetManager& assetManager,
                Functor<void, TYPELIST_1(const Dodge::XmlNode)> setMapSettingsFunc,
                Functor<Dodge::pAsset_t, TYPELIST_1(const Dodge::XmlNode)> factoryFunc,
                Functor<void, TYPELIST_1(Dodge::pAsset_t)> deleteAssetFunc,
                size_t targetMemoryUsage)
         : m_assetManager(assetManager),
           m_setMapSettingsFunc(setMapSettingsFunc),
           m_factoryFunc(factoryFunc),
           m_deleteAssetFunc(deleteAssetFunc),
           m_targetMemUsage(targetMemoryUsage) {}

      void parseMapFile(const std::string& file);
      void update(const Dodge::Vec2f& cameraPos);

      inline const Dodge::Range& getMapBoundary() const;

#ifdef DEBUG
      inline size_t dbg_getMemoryUsage() const;
#endif

   private:
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
            typedef double time_t;

         public:
            void incrRefCount(id_t id);
            void decrRefCount(id_t id);
            void erase(id_t id);
            id_t getBestCandidateForDeletion() const;

         private:
            void insert(id_t id, refCount_t refCount, time_t time);

            std::set<std::pair<refCount_t, std::pair<time_t, id_t> > > m_byRefCount;
            std::map<id_t, std::pair<refCount_t, time_t> > m_byId;

            Dodge::Timer m_timer;
      };

      refCountTable_t m_refCountTable;

      Dodge::EventManager m_eventManager;
      Dodge::AssetManager& m_assetManager;

      Functor<void, TYPELIST_1(const Dodge::XmlNode)> m_setMapSettingsFunc;
      Functor<Dodge::pAsset_t, TYPELIST_1(const Dodge::XmlNode)> m_factoryFunc;
      Functor<void, TYPELIST_1(Dodge::pAsset_t)> m_deleteAssetFunc;

      Dodge::Range m_mapBoundary;
      std::vector<std::vector<mapSegment_t> > m_segments;
      Dodge::Vec2i m_centreSegment;
      Dodge::Vec2f m_segmentSize;

      size_t m_targetMemUsage;
      size_t m_currentMemUsage;

      Dodge::Vec2i getSegment(const Dodge::Vec2f& pos) const;
      void getSegmentRange(const Dodge::Vec2i& indices, Dodge::Range* range) const;

      void loadMapSettings(const Dodge::XmlNode data);
      void loadSegment(const Dodge::Vec2i& indices);
      void loadAssets(const Dodge::XmlNode data, mapSegment_t* segment);
      void parseAssetsFile_r(const std::string& path, mapSegment_t* segment);
      void unloadSegment(const Dodge::Vec2i& indices);
      void unloadRefCountZeroAssets();

      size_t getMemoryUsage() const;
};

#ifdef DEBUG
//===========================================
// MapLoader::dbg_getMemoryUsage
//===========================================
inline size_t MapLoader::dbg_getMemoryUsage() const {
   return getMemoryUsage();
}
#endif

//===========================================
// MapLoader::getMapBoundary
//===========================================
inline const Dodge::Range& MapLoader::getMapBoundary() const {
   return m_mapBoundary;
}


#endif
