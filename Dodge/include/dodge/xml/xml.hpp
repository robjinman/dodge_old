/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_HPP__
#define __XML_HPP__


#include <string>
#include "../../rapidxml/rapidxml.hpp"
#include "XmlNode.hpp"
#include "XmlAttribute.hpp"
#include "XmlDocument.hpp"
#include "XmlException.hpp"


#define XML_NODE_CHECK(msg, node, strName) \
   if (node.isNull() || node.name() != #strName) { \
      throw XmlException(msg + "; Expected '" #strName "' tag", __FILE__, __LINE__); \
   }

#define XML_ATTR_CHECK(msg, attr, strName) \
   if (attr.isNull() || attr.name() != #strName) { \
      throw XmlException(msg + "; Expected '" #strName "' attribute", __FILE__, __LINE__); \
   }


#endif
