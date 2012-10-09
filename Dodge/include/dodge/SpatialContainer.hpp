/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __SPATIAL_CONTAINER_HPP__
#define __SPATIAL_CONTAINER_HPP__


#include "Range.hpp"


namespace Dodge {


#ifdef DEBUG
class Colour;
#endif

template <typename T>
class SpatialContainer {
   public:
      virtual bool insert(T item, const Range& boundingBox) = 0;
      virtual bool remove(T item, const Range& boundingBox) = 0;
      virtual int getNumEntries() const = 0;
      virtual void getEntries(const Range& region, std::vector<T>& entries) const = 0;
      virtual const Range& getBoundary() const = 0;

#ifdef DEBUG
      virtual void dbg_draw(int z, const Colour& col) const = 0;
#endif
};


}


#endif
