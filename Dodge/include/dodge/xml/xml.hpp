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


#define XML_NODE_CHECK(node, strName) \
   if (node.isNull() || node.name() != #strName) { \
      throw Dodge::XmlException(std::string("Expected '" #strName "' tag in file \"") + node.file() + "\"", __FILE__, __LINE__); \
   }

#define XML_ATTR_CHECK(attr, strName) \
   if (attr.isNull() || attr.name() != #strName) { \
      throw Dodge::XmlException(std::string("Expected '" #strName "' attribute in file \"") + attr.file() + "\"", __FILE__, __LINE__); \
   }


#endif
