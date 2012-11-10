#include <memory>
#include <fstream>
#include <sstream>
#include <xml/xml.hpp>


using namespace std;
using namespace rapidxml;


namespace Dodge {


const XmlNode parseXmlDocument(const std::string& file) {
   ifstream fin(file);
   if (!fin.good())
      throw Exception("Error parsing XML file; Bad file", __FILE__, __LINE__);

   // Get file length
   fin.seekg (0, ios::end);
   int len = fin.tellg();
   fin.seekg (0, ios::beg);

   // Load data and add null byte
   unique_ptr<char[]> buf(new char[len + 1]);
   buf[len] = '\0';
   fin.read(buf.get(), len);

   xml_document<char> doc;
   try {
      doc.parse<parse_full ^ parse_comment_nodes>(buf.get());
      fin.close();
      return XmlNode(doc.first_node());
   }
   catch (parse_error& e) {
      fin.close();

      stringstream msg;
      msg << "Error parsing XML file: " << e.what() << " at byte "
         << e.where<char>() - buf.get() << " '" << string(e.where<char>(), 50) << "'";

      throw XmlException(msg.str(), __FILE__, __LINE__);
   }
}


}
