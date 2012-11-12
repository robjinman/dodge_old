/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __PRIMITIVE_FACTORY_HPP__
#define __PRIMITIVE_FACTORY_HPP__


namespace Dodge {


class XmlNode;

class PrimitiveFactory {
   public:
      Primitive* create(const XmlNode data) const;
};


}


#endif
