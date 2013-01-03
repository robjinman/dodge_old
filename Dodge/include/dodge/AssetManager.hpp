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
            inline const std::pair<long, assetMapEntry_t>& operator*();
            inline const std::pair<long, assetMapEntry_t>* operator->();
            inline bool operator==(const iterator& rhs) const;
            inline bool operator!=(const iterator& rhs) const;
            inline iterator& operator++();

         private:
            iterator(assetMap_t::const_iterator i)
               : m_i(i), m_pair(*i) {}

            assetMap_t::const_iterator m_i;
            std::pair<long, assetMapEntry_t> m_pair;
      };

      inline iterator begin() const;
      inline iterator end() const;

      inline void addAsset(long name, pAsset_t obj);
      inline void freeAsset(long name);
      inline void freeAllAssets();
      inline Asset* cloneAsset(long name) const;
      inline pAsset_t getAssetPointer(long name) const;

   private:
      static assetMap_t m_assets;
};

//===========================================
// AssetManager::begin
//===========================================
inline AssetManager::iterator AssetManager::begin() const {
   return iterator(m_assets.begin());
}

//===========================================
// AssetManager::end
//===========================================
inline AssetManager::iterator AssetManager::end() const {
   return iterator(m_assets.end());
}

//===========================================
// AssetManager::addAsset
//===========================================
inline void AssetManager::addAsset(long name, pAsset_t obj) {
   obj->m_id = name;
   m_assets[name] = obj;
}

//===========================================
// AssetManager::freeAsset
//===========================================
inline void AssetManager::freeAsset(long name) {
   m_assets.erase(name);
}

//===========================================
// AssetManager::freeAllAssets
//===========================================
inline void AssetManager::freeAllAssets() {
   m_assets.clear();
}

//===========================================
// AssetManager::cloneAsset
//===========================================
inline Asset* AssetManager::cloneAsset(long name) const {
   assetMap_t::const_iterator i = m_assets.find(name);
   return i != m_assets.end() ? i->second->clone() : NULL;
}

//===========================================
// AssetManager::getAssetPointer
//===========================================
inline pAsset_t AssetManager::getAssetPointer(long name) const {
   assetMap_t::const_iterator i = m_assets.find(name);
   return i != m_assets.end() ? i->second : pAsset_t();
}

//===========================================
// AssetManager::iterator::operator*
//===========================================
inline const std::pair<long, AssetManager::assetMapEntry_t>& AssetManager::iterator::operator*() {
   m_pair = *m_i;
   return m_pair;
}

//===========================================
// AssetManager::iterator::operator->
//===========================================
inline const std::pair<long, AssetManager::assetMapEntry_t>* AssetManager::iterator::operator->() {
   m_pair = *m_i;
   return &m_pair;
}

//===========================================
// AssetManager::iterator::operator==
//===========================================
inline bool AssetManager::iterator::operator==(const iterator& rhs) const {
   return m_i == rhs.m_i;
}

//===========================================
// AssetManager::iterator::operator!=
//===========================================
inline bool AssetManager::iterator::operator!=(const iterator& rhs) const {
   return !(*this == rhs);
}

//===========================================
// AssetManager::iterator::operator++
//===========================================
inline AssetManager::iterator& AssetManager::iterator::operator++() {
   ++m_i;
   return *this;
}


}


#endif
