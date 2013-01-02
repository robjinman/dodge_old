/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2011
 */

#include <cstring>
#include <math.h>
#include <Transformation.hpp>
#include <StringId.hpp>
#include <globals.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Transformation::Transformation
//===========================================
Transformation::Transformation(long name, const std::vector<TransPart>& parts)
   : Asset(internString("Transformation")),
     m_name(name),
     m_parts(parts),
     m_state(STOPPED) {

   init();
}

//===========================================
// Transformation::Transformation
//===========================================
Transformation::Transformation(const XmlNode data)
   : Asset(internString("Transformation")),
     m_state(STOPPED) {

   try {
      XML_NODE_CHECK(data, Transformation);

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, name);
      m_name = internString(attr.getString());

      XmlNode node = data.firstChild();
      while (!node.isNull() && node.name() == "TransPart") {
         TransPart part(node);
         m_parts.push_back(part);

         node = node.nextSibling();
      }

      init();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Transformation; ");
      throw;
   }
}

//===========================================
// Transformation::Transformation
//===========================================
Transformation::Transformation(const Transformation& copy, long name)
   : Asset(internString("Transformation")),
     m_name(name),
     m_state(STOPPED) {

   m_parts = copy.m_parts;

   init();
}

#ifdef DEBUG
//===========================================
// Transformation::dbg_print
//===========================================
void Transformation::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "Transformation\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "name: " << getInternedString(m_name) << "\n";

   for (uint_t p = 0; p < m_parts.size(); ++p) {
      for (int i = 0; i < tab + 1; ++i) out << "\t";
      out << "part " << p << ":\n";

      m_parts[p].dbg_print(out, tab + 2);
   }
}
#endif

//===========================================
// Transformation::getSize
//===========================================
size_t Transformation::getSize() const {
   return sizeof(Transformation)
      + m_parts.size() * sizeof(TransPart);
}

//===========================================
// Transformation::init
//===========================================
void Transformation::init() {
   m_duration = 0.f;

   for (uint_t i = 0; i < m_parts.size(); ++i)
      m_duration += m_parts[i].duration;

   m_numFrames = static_cast<uint_t>(m_duration * gGetTargetFrameRate());

   m_frame = 0;
   m_part = 0;
   if (m_parts.size() > 0) {
      float32_t n = gGetTargetFrameRate() * m_parts[0].duration;
      m_endOfPart = n;

      m_delta.transl.x = m_parts[0].transl.x / n;
      m_delta.transl.y = m_parts[0].transl.y / n;
      m_delta.rot = m_parts[0].rot / n;
      m_delta.scale.x = 1.f + ((m_parts[0].scale.x - 1.f) / n);
      m_delta.scale.y = 1.f + ((m_parts[0].scale.y - 1.f) / n);
   }
}

//===========================================
// Transformation::addPart
//===========================================
void Transformation::addPart(const TransPart& part) {
   m_parts.push_back(part);

   m_duration += m_parts.back().duration;
   m_numFrames = static_cast<uint_t>(m_duration * gGetTargetFrameRate());
}

//===========================================
// Transformation::addParts
//===========================================
void Transformation::addParts(const std::vector<TransPart>& parts) {
   stop();
   m_parts = parts;
   init();
}

//===========================================
// Transformation::clone
//===========================================
Transformation* Transformation::clone() const {
   return new Transformation(*this);
}

//===========================================
// Transformation::play
//===========================================
void Transformation::play() {
   if (m_parts.size() == 0) {
      m_state = STOPPED;
      return;
   }

   switch (m_state) {
      case STOPPED:
         init();
         m_state = PLAYING;
      break;
      case PAUSED:
         m_state = PLAYING;
      break;
      case PLAYING: return;
   }
}

//===========================================
// Transformation::update
//===========================================
const Transformation::delta_t* Transformation::update() {
   if (m_state != PLAYING) return NULL;
   if (m_parts.empty()) return NULL;

   ++m_frame;
   if (m_frame == m_endOfPart) {
      ++m_part;

      if (m_part >= m_parts.size()) {
         m_state = STOPPED;
      }
      else {
         float32_t n = gGetTargetFrameRate() * m_parts[m_part].duration;

         m_endOfPart += n;

         m_delta.transl.x = m_parts[m_part].transl.x / n;
         m_delta.transl.y = m_parts[m_part].transl.y / n;
         m_delta.rot = m_parts[m_part].rot / n;
         m_delta.scale.x = 1.f + ((m_parts[m_part].scale.x - 1.f) / n);
         m_delta.scale.y = 1.f + ((m_parts[m_part].scale.y - 1.f) / n);
      }
   }

   return &m_delta;
}


}
