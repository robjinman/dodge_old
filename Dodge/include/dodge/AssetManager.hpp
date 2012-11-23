/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ASSET_MANAGER_HPP__
#define __ASSET_MANAGER_HPP__


#include <map>
#include <boost/shared_ptr.hpp>
#include "Asset.hpp"


namespace Dodge {


class AssetManager {
   public:
      inline void addAsset(long name, boost::shared_ptr<Asset> obj);
      inline void freeAsset(long name);
      inline void freeAllAssets();
      inline Asset* cloneAsset(long name) const;
      inline boost::shared_ptr<Asset> getAssetPointer(long name) const;

   private:
      typedef boost::shared_ptr<Asset> assetMapEntry_t;
      typedef std::map<long, assetMapEntry_t> assetMap_t;
      static assetMap_t m_assets;
};

//===========================================
// AssetManager::addAsset
//===========================================
void AssetManager::addAsset(long name, boost::shared_ptr<Asset> obj) {
   obj->m_id = name;
   m_assets[name] = obj;
}

//===========================================
// AssetManager::freeAsset
//===========================================
void AssetManager::freeAsset(long name) {
   m_assets.erase(name);
}

//===========================================
// AssetManager::freeAllAssets
//===========================================
void AssetManager::freeAllAssets() {
   m_assets.clear();
}

//===========================================
// AssetManager::cloneAsset
//===========================================
Asset* AssetManager::cloneAsset(long name) const {
   assetMap_t::const_iterator i = m_assets.find(name);
   return i != m_assets.end() ? i->second->clone() : NULL;
}

//===========================================
// AssetManager::getAssetPointer
//===========================================
boost::shared_ptr<Asset> AssetManager::getAssetPointer(long name) const {
   assetMap_t::const_iterator i = m_assets.find(name);
   return i != m_assets.end() ? i->second : boost::shared_ptr<Asset>();
}


}


#endif
