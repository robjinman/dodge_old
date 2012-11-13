/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __XML_DOCUMENT_HPP__
#define __XML_DOCUMENT_HPP__


#include <string>
#include "../../rapidxml/rapidxml.hpp"


namespace Dodge {


class XmlDocument {
   public:
      XmlDocument()
         : m_data(NULL) {}

      XmlNode parse(const std::string& file);

      ~XmlDocument();

   private:
      rapidxml::xml_document<char> m_doc;
      char* m_data;
};


}


#endif
