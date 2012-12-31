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
#include "Item.hpp"


struct gameMap_t {
   gameMap_t() {}

   Dodge::AssetManager        assetManager;
   Dodge::WorldSpace          worldSpace;
   std::map<long, pItem_t>    items;

   Dodge::Range   mapBoundary;
   Dodge::Colour  bgColour;
};

class MapLoader {
   public:
      void parseMapFile(const std::string& file, gameMap_t* gameMap);
      void update(const Dodge::Vec2f& cameraPos);

   private:
      gameMap_t* m_gameMap;
      long m_fillerTileId;

      struct mapSegment_t {
         mapSegment_t()
            : xmlDocument(new Dodge::XmlDocument) {}

         std::string filePath;
         boost::shared_ptr<Dodge::XmlDocument> xmlDocument;
         Dodge::XmlNode data;

         std::vector<long> assetIds;
      };

      std::vector<std::vector<mapSegment_t> > m_segments;   // 2d array of map segments.
      Dodge::Vec2i m_centreSegment;
      Dodge::Vec2f m_segmentSize;

      std::map<long, unsigned int> m_refCounts;

      Dodge::Vec2i getSegment(const Dodge::Vec2f& pos) const;
      void getSegmentRange(const Dodge::Vec2i& indices, Dodge::Range* range) const;

      void loadMapSettings(const Dodge::XmlNode data);
      void loadSegment(const Dodge::Vec2i& indices);
      void parseMapSegment(mapSegment_t& segment);
      void loadAssets_r(const Dodge::XmlNode data, mapSegment_t* segment, int depth = 0);
      boost::shared_ptr<Dodge::Asset> constructAsset(const Dodge::XmlNode data, long proto, bool addToWorld);
      pItem_t constructItem(const Dodge::XmlNode data) const;
      void unloadSegment(const Dodge::Vec2i& indices);
      void unloadRefCountZeroAssets();
      void fillRange(const Dodge::Range& range);
};


#endif
