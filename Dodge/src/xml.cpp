/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <xml.hpp>
#include <Exception.hpp>


using namespace rapidxml;


namespace Dodge {


//===========================================
// nthChild
//===========================================
const xml_node<>* nthChild(const xml_node<>* node, uint_t n) {
   if (!node)
      throw Exception("Error fetching child node; Node is NULL", __FILE__, __LINE__);

   xml_node<>* c = node->first_node();

   for (uint_t i = 0; i < n; ++i) {
      if (!c)
         throw Exception("Error fetching child node; Node doesn't exist", __FILE__, __LINE__);

      c = c->next_sibling();
   }

   if (!c)
      throw Exception("Error fetching child node; Node doesn't exist", __FILE__, __LINE__);

   return c;
}


}
