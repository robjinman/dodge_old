#include <cstring>
#include <xml/XmlNode.hpp>
#include <xml/XmlDocument.hpp>
#include <definitions.hpp>
#include <sstream>


using namespace std;


namespace Dodge {


//===========================================
// XmlNode::copy_r
//
// Because rapidxml doesn't provide a deep copy function (clone_node is inadequate
// because it doesn't copy strings).
//===========================================
rapidxml::xml_node<>* XmlNode::copy_r(rapidxml::xml_node<>* node, rapidxml::xml_document<char>* doc) {
   // Copy top level node
   char* name = doc->allocate_string(node->name(), node->name_size());
   char* val = doc->allocate_string(node->value(), node->value_size());

   rapidxml::xml_node<>* nodeCpy = doc->allocate_node(node->type(), name, val, node->name_size(), node->value_size());

   rapidxml::xml_attribute<>* attr = node->first_attribute();
   while (attr != NULL) {
      char* name = doc->allocate_string(attr->name(), attr->name_size());
      char* val = doc->allocate_string(attr->value(), attr->value_size());

      rapidxml::xml_attribute<>* attrCpy = doc->allocate_attribute(name, val, attr->name_size(), attr->value_size());
      nodeCpy->append_attribute(attrCpy);

      attr = attr->next_attribute();
   }

   // Copy children
   rapidxml::xml_node<>* ch = node->first_node();
   while (ch != NULL) {
      rapidxml::xml_node<>* chCpy = XmlNode::copy_r(ch, doc);
      nodeCpy->append_node(chCpy);

      ch = ch->next_sibling();
   }

   return nodeCpy;
}

//===========================================
// XmlNode::XmlNode
//===========================================
XmlNode::XmlNode(XmlDocument& doc, const string& name)
   : m_doc(doc.m_doc) {

   auto pDoc = m_doc.lock();
   assert(pDoc);

   char* strName = pDoc->allocate_string(name.data(), name.length());
   m_node = pDoc->allocate_node(rapidxml::node_element, strName, 0, name.length());
   m_file = "n/a";
}

//===========================================
// XmlNode::copy
//===========================================
XmlNode XmlNode::copy(XmlDocument& doc) const {
   rapidxml::xml_node<>* node = XmlNode::copy_r(m_node, doc.m_doc.get());
   return XmlNode(doc.m_doc, node, m_file);
}

//===========================================
// XmlNode::getInt
//===========================================
int XmlNode::getInt() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   stringstream ss;
   ss.write(m_node->value(), m_node->value_size());

   int i;
   ss >> i;

   if (ss.fail())
      throw XmlException("Expected integer", __FILE__, __LINE__);

   return i;
}

//===========================================
// XmlNode::getLong
//===========================================
long XmlNode::getLong() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   stringstream ss;
   ss.write(m_node->value(), m_node->value_size());

   long i;
   ss >> i;

   if (ss.fail())
      throw XmlException("Expected long integer", __FILE__, __LINE__);

   return i;
}

//===========================================
// XmlNode::getFloat
//===========================================
float XmlNode::getFloat() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   stringstream ss;
   ss.write(m_node->value(), m_node->value_size());

   float f;
   ss >> f;

   if (ss.fail())
      throw XmlException("Expected float", __FILE__, __LINE__);

   return f;
}

//===========================================
// XmlNode::getDouble
//===========================================
double XmlNode::getDouble() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   stringstream ss;
   ss.write(m_node->value(), m_node->value_size());

   double d;
   ss >> d;

   if (ss.fail())
      throw XmlException("Expected double", __FILE__, __LINE__);

   return d;
}

//===========================================
// XmlNode::getBool
//===========================================
bool XmlNode::getBool() const {
   if (isNull())
      throw XmlException("Node is NULL", __FILE__, __LINE__);

   if (strncmp(m_node->value(), "true", m_node->value_size()) == 0)
      return true;
   else if (strncmp(m_node->value(), "false", m_node->value_size()) == 0)
      return false;

   throw XmlException("Expected 'true' or 'false'", __FILE__, __LINE__);
}

//===========================================
// XmlNode::addNode
//===========================================
XmlNode XmlNode::addNode(const string& name) {
   auto doc = m_doc.lock();
   assert(doc);

   char* str = doc->allocate_string(name.data());
   rapidxml::xml_node<>* node = doc->allocate_node(rapidxml::node_element, str, 0, name.length());

   m_node->append_node(node);

   return XmlNode(m_doc, node, "n/a");
}

//===========================================
// XmlNode::addNode
//===========================================
XmlNode XmlNode::addNode(const XmlNode& node) {
   auto doc = m_doc.lock();
   assert(doc);

   rapidxml::xml_node<>* cpy;

   if (doc == node.m_doc.lock())
      cpy = node.m_node;
   else
      cpy = XmlNode::copy_r(node.m_node, doc.get());

   m_node->append_node(cpy);

   return XmlNode(m_doc, cpy, node.m_file);
}

//===========================================
// XmlNode::addAttribute
//===========================================
XmlAttribute XmlNode::addAttribute(const string& name, const string& value) {
   auto doc = m_doc.lock();
   assert(doc);

   char* strName = doc->allocate_string(name.data(), name.length());
   char* strValue = doc->allocate_string(value.data(), value.length());

   rapidxml::xml_attribute<>* attr = doc->allocate_attribute(strName, strValue, name.length(), value.length());

   m_node->append_attribute(attr);

   return XmlAttribute(m_doc, attr, "n/a");
}

//===========================================
// XmlNode::setValue
//===========================================
void XmlNode::setValue(const string& val) {
   auto doc = m_doc.lock();
   assert(doc);

   char* str = doc->allocate_string(val.data(), val.length());
   m_node->value(str, val.length());
}

//===========================================
// XmlNode::nthChild
//===========================================
XmlNode XmlNode::nthChild(uint_t n) const {
   if (isNull()) throw XmlException("Node is NULL", __FILE__, __LINE__);

   XmlNode c = firstChild();
   for (uint_t i = 0; i < n; ++i) c = c.nextSibling();

   return c;
}


}
