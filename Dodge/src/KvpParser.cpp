/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <fstream>
#include <cstdio>
#include <string>
#include <KvpParser.hpp>
#include <Exception.hpp>


using namespace std;


namespace Dodge {


static const unsigned int BUF_SIZE = 256;


//===========================================
// KvpParser::getMetaData
//
// Gets strings that start with '!'.
//===========================================
const string& KvpParser::getMetaData(unsigned int index) const {
   if (index >= m_metaData.size())
      throw Exception("Error retrieving meta-data string; index out of bounds",
         __FILE__, __LINE__);

   return m_metaData[index];
}

//===========================================
// KvpParser::getValue
//
// Retrieve value by key.
//===========================================
const string& KvpParser::getValue(const string& key) const {
   map<string, string>::const_iterator it = m_data.find(key);
   if (it == m_data.end())
      throw Exception("Error retrieving key-value pair; key not found", __FILE__, __LINE__);

   return it->second;
}

//===========================================
// KvpParser::getLine
//===========================================
string KvpParser::getLine(ifstream& fin) const {
   char buf[BUF_SIZE];

   fin.getline(buf, BUF_SIZE);
   for (unsigned int i = 0; i < BUF_SIZE; i++)
      if (buf[i] == '#') buf[i] = '\0';

   string str(buf);
   while (str[0] == ' ' || str[0] == '\t') str.erase(0, 1);

   return str;
}

//===========================================
// KvpParser::parseFile
//===========================================
void KvpParser::parseFile(const string& file) {
   ifstream fin(file.data());
   if (!fin.is_open())
      throw Exception("Could not open file", __FILE__, __LINE__);

   char str[BUF_SIZE];
   string buf;

   buf = getLine(fin);
   while (sscanf(buf.data(), "!%s", str) == 1) {
      m_metaData.push_back(string(str));
      buf = getLine(fin);
   }
   while (!fin.eof()) {
      if (buf.length() > 0) {
         char strKey[BUF_SIZE], strVal[BUF_SIZE];

         if (sscanf(buf.data(), "%s = %s", strKey, strVal) != 2)
            throw Exception("Error parsing file", __FILE__, __LINE__);

         m_data[string(strKey)] = strVal;
      }
      buf = getLine(fin);
   }

   fin.close();
}


}
