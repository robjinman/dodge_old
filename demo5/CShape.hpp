#ifndef __CSHAPE_HPP__
#define __CSHAPE_HPP__


#include <boost/shared_ptr.hpp>
#include "dodge/Shape.hpp"
#include "dodge/xml/xml.hpp"
#include "Item.hpp"


class CShape : public Item, public Dodge::Shape {
   public:
      CShape(const Dodge::XmlNode data);

      CShape(const CShape& copy)
         : Entity(copy),
           Item(copy),
           Shape(copy) {}

      CShape(const CShape& copy, long name)
         : Entity(copy, name),
           Item(copy, name),
           Shape(copy, name) {}

      virtual CShape* clone() const;

      virtual void assignData(const Dodge::XmlNode data);

      virtual ~CShape() {}
};

typedef boost::shared_ptr<CShape> pCShape_t;


#endif
