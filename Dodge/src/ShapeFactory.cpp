/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <math/shapes/shapes.hpp>
#include <xml/xml.hpp>
#include <ShapeFactory.hpp>


namespace Dodge {


//===========================================
// ShapeFactory::ShapeFactory
//===========================================
Shape* ShapeFactory::create(const XmlNode data) const {
   if (data.isNull())
      throw XmlException("Error creating shape of unspecified type", __FILE__, __LINE__);

   if (data.name() == "Ellipse") return new Ellipse(data);
   else if (data.name() == "LineSegment") return new LineSegment(data);
   else if (data.name() == "Polygon") return new Polygon(data);
   else if (data.name() == "Quad") return new Quad(data);
   else
      throw XmlException("Error creating shape of unknown type", __FILE__, __LINE__);
}


}
