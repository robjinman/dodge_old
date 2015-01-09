/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ASSET_MANAGER_HPP__
#define __ASSET_MANAGER_HPP__


#include <map>
#include "Asset.hpp"


namespace Dodge {


class AssetManager {
   private:
      typedef pAsset_t assetMapEntry_t;
      typedef std::map<long, assetMapEntry_t> assetMap_t;

   public:
      class iterator {
         friend class AssetManager;

         public:
            const std::pair<long, assetMapEntry_t>& operator*();
            const std::pair<long, assetMapEntry_t>* operator->();
            bool operator==(const iterator& rhs) const;
            bool operator!=(const iterator& rhs) const;
            iterator& operator++();

         private:
            iterator(assetMap_t::const_iterator i, bool end = false);

            assetMap_t::const_iterator m_i;
            std::pair<long, assetMapEntry_t> m_pair;
      };

      iterator begin() const;
      iterator end() const;

      void addAsset(long name, pAsset_t obj);
      void freeAsset(long name);
      void freeAllAssets();
      Asset* cloneAsset(long name) const;
      pAsset_t getAssetPointer(long name) const;

   private:
      static assetMap_t m_assets;
};


}


#endif
