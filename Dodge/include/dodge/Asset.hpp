/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ASSET_HPP__
#define __ASSET_HPP__


#include <boost/shared_ptr.hpp>


namespace Dodge {


class Asset {
   friend class AssetManager;

   public:
      Asset(long typeId)
         : m_typeId(typeId) {}

      virtual Asset* clone() const = 0;
      virtual size_t getSize() const = 0;

      inline long getAssetId() const;
      inline long getAssetTypeId() const;

   private:
      long m_id;
      long m_typeId;
};

typedef boost::shared_ptr<Asset> pAsset_t;

//===========================================
// Asset::getAsseId
//===========================================
inline long Asset::getAssetId() const {
   return m_id;
}

//===========================================
// Asset::getAssetTypeId
//===========================================
inline long Asset::getAssetTypeId() const {
   return m_typeId;
}


}


#endif
