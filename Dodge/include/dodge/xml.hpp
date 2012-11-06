/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_HPP__
#define __XML_HPP__


#include "definitions.hpp"
#include "rapidxml/rapidxml.hpp"


namespace Dodge {


const rapidxml::xml_node<>* nthChild(const rapidxml::xml_node<>* node, uint_t n);


}


#endif
