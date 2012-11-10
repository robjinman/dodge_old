/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_HPP__
#define __XML_HPP__


#include "rapidxml/rapidxml.hpp"
#include "XmlNode.hpp"
#include "XmlAttribute.hpp"
#include "XmlException.hpp"


namespace Dodge {


const XmlNode parseXmlDocument(const std::string& file);


}


#endif
