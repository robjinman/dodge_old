/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <fstream>
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
#include <KvpParser.hpp>
#include <Exception.hpp>
#include <definitions.hpp>


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
string KvpParser::getValue(const string& key) const {
   map<string, string>::const_iterator it = m_data.find(key);
   if (it == m_data.end()) return string("");

   return it->second;
}

//===========================================
// KvpParser::getLine
//===========================================
string KvpParser::getLine(ifstream& fin) const {
   char buf[BUF_SIZE];
   memset(buf, 0, BUF_SIZE);

   fin.getline(buf, BUF_SIZE);
   for (unsigned int i = 0; i < BUF_SIZE; i++)
      if (buf[i] == '#') buf[i] = '\0';

   string str(buf);
   while (str[0] == ' ' || str[0] == '\t') str.erase(0, 1);

   return str;
}

//===========================================
// KvpParser::insertPair
//===========================================
void KvpParser::insertPair(const string& key, const string& value) {
   m_data[key] = value;
}

//===========================================
// KvpParser::removePair
//===========================================
void KvpParser::removePair(const string& key) {
   m_data.erase(key);
}

//===========================================
// KvpParser::addMetaData
//===========================================
void KvpParser::addMetaData(const std::string& value, int index = -1) {
   if (index == -1)
      m_metaData.push_back(value);
   else
      m_metaData.insert(m_metaData.begin() + index, value);
}

//===========================================
// KvpParser::eraseMetaData
//===========================================
void KvpParser::eraseMetaData(unsigned int index) {
   m_metaData.erase(m_metaData.begin() + index);
}

//===========================================
// KvpParser::writeToFile
//===========================================
void KvpParser::writeToFile(const string& file) const {
   ofstream fout(file.data());

   if (!fout.good()) {
      fout.close();

      stringstream msg;
      msg << "Error writing to file " << file;
      throw Exception(msg.str(), __FILE__, __LINE__);
   }

   for (uint_t i = 0; i < m_metaData.size(); ++i)
      fout << "!" << m_metaData[i] << "\n";

   fout << "\n";

   for (auto i = m_data.begin(); i != m_data.end(); ++i) {
      fout << i->first << " = " << i->second << "\n";
   }

   fout.close();
}

//===========================================
// KvpParser::parseFile
//===========================================
void KvpParser::parseFile(const string& file) {
   ifstream fin(file.data());
   if (!fin.is_open()) return;

   char str[BUF_SIZE];
   string buf;
   stringstream formatStr;

   buf = getLine(fin);

   formatStr.str("");
   formatStr << "!%" << BUF_SIZE - 1 << "s";

   while (sscanf(buf.data(), formatStr.str().data(), str) == 1) {
      m_metaData.push_back(string(str));
      buf = getLine(fin);
   }
   while (!fin.eof()) {
      if (buf.length() > 0) {
         char strKey[BUF_SIZE], strVal[BUF_SIZE];

         formatStr.str("");
         formatStr << "%" << BUF_SIZE - 1 << "s = %" << BUF_SIZE - 1 << "s";

         if (sscanf(buf.data(), formatStr.str().data(), strKey, strVal) != 2)
            throw Exception("Error parsing file", __FILE__, __LINE__);

         m_data[string(strKey)] = strVal;
      }
      buf = getLine(fin);
   }

   fin.close();
}


}
