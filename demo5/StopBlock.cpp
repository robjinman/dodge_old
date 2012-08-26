#include <cstring>
#include <Dodge/core/Exception.hpp>
#include "rapidxml.hpp"
#include "StopBlock.hpp"


using namespace rapidxml;
using namespace Dodge;


//===========================================
// StopBlock::assignData
//===========================================
void StopBlock::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "StopBlock") != 0)
      throw Exception("Error parsing XML for instance of class StopBlock", __FILE__, __LINE__);

   const xml_node<>* node = data->first_node();
   if (node) Item::assignData(node);
}
