#ifndef __GRAVITY_REGION_HPP__
#define __GRAVITY_REGION_HPP__


#include <boost/shared_ptr.hpp>
#include "dodge/Shape.hpp"
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class GravityRegion : public Item, public Dodge::Shape {
   public:
      GravityRegion(const Dodge::XmlNode data);

      GravityRegion(const GravityRegion& copy)
         : Entity(copy),
           Item(copy),
           Shape(copy) {}

      GravityRegion(const GravityRegion& copy, long name)
         : Entity(copy, name),
           Item(copy, name),
           Shape(copy, name) {}

      virtual GravityRegion* clone() const;

      virtual void assignData(const Dodge::XmlNode data);

      virtual ~GravityRegion() {}
};

typedef boost::shared_ptr<GravityRegion> pGravityRegion_t;


#endif
