/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
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
                Functor<void, TYPELIST_1(Dodge::pAsset_t)> deleteAssetFunc)
         : m_assetManager(assetManager),
           m_setMapSettingsFunc(setMapSettingsFunc),
           m_factoryFunc(factoryFunc),
           m_deleteAssetFunc(deleteAssetFunc) {}

      void parseMapFile(const std::string& file);
      void update(const Dodge::Vec2f& cameraPos);

      inline const Dodge::Range& getMapBoundary() const;

#ifdef DEBUG
      size_t dbg_getMemoryUsage() const;
#endif

   private:
      Dodge::EventManager m_eventManager;
      Dodge::AssetManager& m_assetManager;

      struct mapSegment_t {
         mapSegment_t() {}

         std::string filePath;
         std::vector<long> assetIds;
      };

      Functor<void, TYPELIST_1(const Dodge::XmlNode)> m_setMapSettingsFunc;
      Functor<Dodge::pAsset_t, TYPELIST_1(const Dodge::XmlNode)> m_factoryFunc;
      Functor<void, TYPELIST_1(Dodge::pAsset_t)> m_deleteAssetFunc;

      Dodge::Range m_mapBoundary;
      std::vector<std::vector<mapSegment_t> > m_segments;   // 2d array of map segments.
      Dodge::Vec2i m_centreSegment;
      Dodge::Vec2f m_segmentSize;

      std::map<long, unsigned int> m_refCounts;

      Dodge::Vec2i getSegment(const Dodge::Vec2f& pos) const;
      void getSegmentRange(const Dodge::Vec2i& indices, Dodge::Range* range) const;

      void loadMapSettings(const Dodge::XmlNode data);
      void loadSegment(const Dodge::Vec2i& indices);
      void loadAssets(const Dodge::XmlNode data, mapSegment_t* segment);
      void parseAssetsFile_r(const std::string& path, mapSegment_t* segment);
      void unloadSegment(const Dodge::Vec2i& indices);
      void unloadRefCountZeroAssets();
};

//===========================================
// MapLoader::getMapBoundary
//===========================================
inline const Dodge::Range& MapLoader::getMapBoundary() const {
   return m_mapBoundary;
}


#endif
