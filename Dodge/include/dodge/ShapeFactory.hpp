/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __SHAPE_FACTORY_HPP__
#define __SHAPE_FACTORY_HPP__


namespace Dodge {


class XmlNode;

class ShapeFactory {
   public:
      Shape* create(const XmlNode data) const;
};


}


#endif
