/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ASSET_HPP__
#define __ASSET_HPP__


namespace Dodge {


class Asset {
   friend class AssetManager;

   public:
      virtual Asset* clone() const = 0;
      inline long getAssetId() const;

   private:
      long m_id;
};

inline long Asset::getAssetId() const { return m_id; }


}


#endif
