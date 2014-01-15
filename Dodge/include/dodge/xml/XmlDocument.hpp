/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_DOCUMENT_HPP__
#define __XML_DOCUMENT_HPP__


#include <string>
#include <ostream>
#include <memory>
#include "../../rapidxml/rapidxml.hpp"


namespace Dodge {


class XmlNode;

class XmlDocument {
   friend class XmlNode;

   public:
      XmlDocument();
      XmlDocument(const XmlDocument& cpy);

      const XmlNode parse(const std::string& file);
      const XmlNode parse(const char* str, size_t len);

      const XmlNode firstNode();
      inline bool isNull() const;

      void print(std::ostream& out) const;
      void print(std::string& out) const;

      XmlNode addNode(const XmlNode& node);
      XmlNode addNode(const std::string& name);

      ~XmlDocument();

   private:
      XmlNode parse_();

      std::shared_ptr<rapidxml::xml_document<char> > m_doc;
      char* m_buf;
      int m_bufLen;
      std::string m_file;
};

//===========================================
// XmlDocument::isNull
//===========================================
inline bool XmlDocument::isNull() const {
   return m_doc->first_node() == NULL;
}


}


#endif
