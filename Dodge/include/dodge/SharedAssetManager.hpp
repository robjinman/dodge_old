/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __SHARED_ASSET_MANAGER_HPP__
#define __SHARED_ASSET_MANAGER_HPP__


#include <map>
#include <boost/shared_ptr.hpp>
#include "Asset.hpp"


namespace Dodge {


class SharedAssetManager {
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
// SharedAssetManager::addAsset
//===========================================
void SharedAssetManager::addAsset(long name, boost::shared_ptr<Asset> obj) {
   m_assets[name] = std::move(obj);
}

//===========================================
// SharedAssetManager::freeAsset
//===========================================
void SharedAssetManager::freeAsset(long name) {
   m_assets.erase(name);
}

//===========================================
// SharedAssetManager::freeAllAssets
//===========================================
void SharedAssetManager::freeAllAssets() {
   m_assets.clear();
}

//===========================================
// SharedAssetManager::cloneAsset
//===========================================
Asset* SharedAssetManager::cloneAsset(long name) const {
   assetMap_t::const_iterator i = m_assets.find(name);
   return i != m_assets.end() ? i->second->clone() : NULL;
}

//===========================================
// SharedAssetManager::getAssetPointer
//===========================================
boost::shared_ptr<Asset> SharedAssetManager::getAssetPointer(long name) const {
   assetMap_t::const_iterator i = m_assets.find(name);
   return i != m_assets.end() ? i->second : boost::shared_ptr<Asset>();
}


}


#endif
