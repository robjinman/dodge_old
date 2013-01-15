/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <AssetManager.hpp>
#include <Exception.hpp>


namespace Dodge {


AssetManager::assetMap_t AssetManager::m_assets;


//===========================================
// AssetManager::iterator::iterator
//===========================================
AssetManager::iterator::iterator(assetMap_t::const_iterator i, bool end)
   : m_i(i) {

   if (!end) m_pair = *i;
}

//===========================================
// AssetManager::begin
//===========================================
AssetManager::iterator AssetManager::begin() const {
   return iterator(m_assets.begin());
}

//===========================================
// AssetManager::end
//===========================================
AssetManager::iterator AssetManager::end() const {
   return iterator(m_assets.end(), true);
}

//===========================================
// AssetManager::addAsset
//===========================================
void AssetManager::addAsset(long name, pAsset_t obj) {
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
pAsset_t AssetManager::getAssetPointer(long name) const {
   assetMap_t::const_iterator i = m_assets.find(name);
   return i != m_assets.end() ? i->second : pAsset_t();
}

//===========================================
// AssetManager::iterator::operator*
//===========================================
const std::pair<long, AssetManager::assetMapEntry_t>& AssetManager::iterator::operator*() {
   if (m_i == m_assets.end())
      throw Exception("Attempt to dereference 'end' iterator", __FILE__, __LINE__);

   m_pair = *m_i;
   return m_pair;
}

//===========================================
// AssetManager::iterator::operator->
//===========================================
const std::pair<long, AssetManager::assetMapEntry_t>* AssetManager::iterator::operator->() {
   if (m_i == m_assets.end())
      throw Exception("Attempt to dereference 'end' iterator", __FILE__, __LINE__);

   m_pair = *m_i;
   return &m_pair;
}

//===========================================
// AssetManager::iterator::operator==
//===========================================
bool AssetManager::iterator::operator==(const iterator& rhs) const {
   return m_i == rhs.m_i;
}

//===========================================
// AssetManager::iterator::operator!=
//===========================================
bool AssetManager::iterator::operator!=(const iterator& rhs) const {
   return !(*this == rhs);
}

//===========================================
// AssetManager::iterator::operator++
//===========================================
AssetManager::iterator& AssetManager::iterator::operator++() {
   ++m_i;
   return *this;
}

}
