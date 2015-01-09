/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __KVP_PARSER_HPP__
#define __KVP_PARSER_HPP__


#include <string>
#include <map>
#include <vector>
#include <stdexcept>


namespace Dodge {


// Parses a text file of key-value pairs.
class KvpParser {
   public:
      void parseFile(const std::string& file);

      std::string getValue(const std::string& key) const;

      void insertPair(const std::string& key, const std::string& value);
      void removePair(const std::string& key);

      const std::string& getMetaData(unsigned int index) const;
      void addMetaData(const std::string& value, int index);
      void eraseMetaData(unsigned int index);

      void writeToFile(const std::string& file) const;

   private:
      std::map<std::string, std::string> m_data;
      std::vector<std::string> m_metaData;

      std::string getLine(std::ifstream& fin) const;
};


}


#endif /*!__KVP_PARSER_HPP__*/
