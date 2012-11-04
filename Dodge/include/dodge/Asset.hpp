#ifndef __ASSET_HPP__
#define __ASSET_HPP__


namespace Dodge {


class Asset {
   public:
      virtual Asset* clone() const = 0;
};


}


#endif
