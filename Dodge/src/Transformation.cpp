/*
 * Author: Rob Jinman <jinmane@gmail.com>
 * Date: 2011
 */

#include <cstring>
#include <math.h>
#include <Transformation.hpp>
#include <StringId.hpp>


using namespace std;
using namespace rapidxml;


namespace Dodge {


#ifdef DEBUG
//===========================================
// Transformation::dbg_print
//===========================================
void Transformation::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; i++) out << "\t";
   out << "Transformation\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "name: " << getInternedString(m_name) << "\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "rate: " << m_rate << "\n";

   for (int i = 0; i < tab + 1; i++) out << "\t";
   out << "smooth: " << m_smooth << "\n";

   for (unsigned int f = 0; f < m_frames.size(); f++) {
      for (int i = 0; i < tab + 1; i++) out << "\t";
      out << "frame " << f << ":\n";

      m_frames[f].dbg_print(out, tab + 2);
   }
}
#endif

//===========================================
// Transformation::Transformation
//===========================================
Transformation::Transformation(long name, double rate, const std::vector<TransFrame>& frames)
   : m_name(name), m_rate(rate), m_smooth(1), m_frames(frames), m_state(STOPPED),
     m_frameReady(false), m_tmpFrame(Vec2f(0.0, 0.0), 0.0, Vec2f(1.0, 1.0)) {}

//===========================================
// Transformation::assignData
//===========================================
void Transformation::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Transformation") != 0)
      throw Exception("Error parsing XML for instance of class Transformation", __FILE__, __LINE__);

   xml_attribute<>* attr = data->first_attribute();
   if (attr && strcmp(attr->name(), "name") == 0) {
      m_name = internString(attr->value());
   }

   xml_node<>* node = data->first_node();
   if (node && strcmp(node->name(), "rate") == 0) {
      sscanf(node->value(), "%lf", &m_rate);
      node = node->next_sibling();
   }
   if (node && strcmp(node->name(), "smooth") == 0) {
      sscanf(node->value(), "%d", &m_smooth);
      node = node->next_sibling();
   }
   while (node && strcmp(node->name(), "TransFrame") == 0) {
      TransFrame frame;
      frame.assignData(node);
      m_frames.push_back(frame);

      node = node->next_sibling();
   }
}

//===========================================
// Transformation::play
//===========================================
void Transformation::play() {
   switch (m_state) {
      case STOPPED:
         m_timer.reset();
         m_prev = m_timer.getTime();
         m_current = 0;
         m_state = PLAYING;
      break;
      case PAUSED:
         m_timer.reset();
         m_state = PLAYING;
         m_prev = m_timer.getTime();
      break;
      case PLAYING: return;
   }
}

//===========================================
// Transformation::update
//===========================================
void Transformation::update() {
   if (m_state != PLAYING) return;

   double time = m_timer.getTime();
   double diff = time - m_prev;
   double dt = 1.0 / (m_rate * static_cast<double>(m_smooth));
   if (diff >= dt) {
      double extra = diff - dt;

      double dx = 0, dy = 0, da = 0;
      Vec2f dScale(1, 1);
      do {
         dx += m_frames[m_current / m_smooth].delta.x / static_cast<double>(m_smooth);
         dy += m_frames[m_current / m_smooth].delta.y / static_cast<double>(m_smooth);
         da += m_frames[m_current / m_smooth].rot / static_cast<double>(m_smooth);

         if (m_current % m_smooth == 0) {
            dScale.x *= m_frames[m_current / m_smooth].scale.x;
            dScale.y *= m_frames[m_current / m_smooth].scale.y;
         }

         m_current++;
         if (m_current == m_frames.size() * m_smooth) {
            m_state = STOPPED;
            break;
         }

         if (extra >= dt) extra -= dt;
      } while (extra >= dt); // Catch up if there are missed frames

      m_tmpFrame = TransFrame(Vec2f(dx, dy), da, dScale);
      m_frameReady = true;

      m_prev = time - extra;
   }
}


}
