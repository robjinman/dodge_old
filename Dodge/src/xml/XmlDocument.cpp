/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <memory>
#include <fstream>
#include <sstream>
#include <cstring>
#include <xml/xml.hpp>
#include <rapidxml/rapidxml_print.hpp>


using namespace std;
using namespace rapidxml;


namespace Dodge {


//===========================================
// XmlDocument::XmlDocument
//===========================================
XmlDocument::XmlDocument()
   : m_buf(NULL),
     m_bufLen(0) {

   m_doc = shared_ptr<xml_document<char> >(new xml_document<char>);
   m_file = "n/a";
}

//===========================================
// XmlDocument::XmlDocument
//
// Shallow copy
//===========================================
XmlDocument::XmlDocument(const XmlDocument& cpy)
   : m_doc(cpy.m_doc),
     m_buf(cpy.m_buf),
     m_bufLen(cpy.m_bufLen),
     m_file(cpy.m_file) {}

//===========================================
// XmlDocument::parse
//===========================================
const XmlNode XmlDocument::parse(const string& file) {
   ifstream fin(file);
   if (!fin.good())
      throw Exception("Error parsing XML file; Bad file (" + file + ")", __FILE__, __LINE__);

   fin.seekg(0, std::ios::end);
   int size = fin.tellg();
   fin.seekg(0, std::ios::beg);

   if (m_buf) delete[] m_buf;
   m_buf = new char[size + 1];
   m_buf[size] = '\0';

   m_bufLen = size;

   fin.read(m_buf, size);

   if (!fin)
      throw Exception("Error parsing XML file; Bad file (" + file + ")", __FILE__, __LINE__);

   m_file = file;

   fin.close();

   return parse_();
}

//===========================================
// XmlDocument::parse
//===========================================
const XmlNode XmlDocument::parse(const char* str, size_t len) {
   if (m_buf) delete[] m_buf;

   m_buf = new char[len + 1];
   m_buf[len] = '\0';

   m_bufLen = len;

   memcpy(m_buf, str, len);

   return parse_();
}

//===========================================
// XmlDocument::parse_
//===========================================
XmlNode XmlDocument::parse_() {
   assert(m_buf);
   assert(m_bufLen > 0);

   try {
      char* dat = const_cast<char*>(m_buf);

      m_doc->parse<parse_full ^ parse_comment_nodes>(dat);
      return XmlNode(m_doc, m_doc->first_node(), m_file);
   }
   catch (parse_error& e) {
      stringstream msg;
      msg << "Error parsing XML file (" << m_file << "): " << e.what() << " at byte "
         << e.where<char>() - m_buf << " '" << string(e.where<char>(), 50) << "'";

      throw XmlException(msg.str(), __FILE__, __LINE__);
   }
}

//===========================================
// XmlDocument::firstNode
//===========================================
const XmlNode XmlDocument::firstNode() {
   return isNull() ? XmlNode() : XmlNode(m_doc, m_doc->first_node(), m_file);
}

//===========================================
// XmlDocument::addNode
//===========================================
XmlNode XmlDocument::addNode(const std::string& name) {
   char* str = m_doc->allocate_string(name.data());
   xml_node<>* node = m_doc->allocate_node(rapidxml::node_element, str, 0, name.length());

   m_doc->append_node(node);

   return XmlNode(m_doc, node, "");
}

//===========================================
// XmlDocument::addNode
//===========================================
XmlNode XmlDocument::addNode(const XmlNode& node) {
   rapidxml::xml_node<>* cpy;

   if (m_doc == node.m_doc.lock())
      cpy = node.m_node;
   else
      cpy = XmlNode::copy_r(node.m_node, m_doc.get());

   m_doc->append_node(cpy);

   return XmlNode(m_doc, cpy, node.m_file);
}

//===========================================
// XmlDocument::print
//===========================================
void XmlDocument::print(ostream& out) const {
   out << *m_doc;
}

//===========================================
// XmlDocument::print
//===========================================
void XmlDocument::print(string& out) const {
   out.clear();
   rapidxml::print(std::back_inserter(out), *m_doc.get(), 0);
}

//===========================================
// XmlDocument::XmlDocument
//===========================================
XmlDocument::~XmlDocument() {
   delete[] m_buf;
}


}
