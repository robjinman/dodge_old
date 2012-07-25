/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <Animation.hpp>
#include <StringId.hpp>


using namespace std;
using namespace rapidxml;


namespace Dodge {


#ifdef DEBUG
//===========================================
// Animation::dbg_print
//===========================================
void Animation::dbg_print(std::ostream& out, int tab) const {
   for (int i = 0; i < tab; ++i) out << "\t";
   out << "Animation\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "name: " << getInternedString(m_name) << "\n";

   for (int i = 0; i < tab + 1; ++i) out << "\t";
   out << "rate: " << m_rate << "\n";

   for (unsigned int f = 0; f < m_frames.size(); f++) {
      for (int i = 0; i < tab + 1; ++i) out << "\t";
      out << "frame :\n";

      for (int i = 0; i < tab + 2; ++i) out << "\t";
      out << "pos: (" << m_frames[f].pos.x << ", " << m_frames[f].pos.y << ")\n";

      for (int i = 0; i < tab + 2; ++i) out << "\t";
      out << "dim: (" << m_frames[f].dim.x << ", " << m_frames[f].dim.y << ")\n";

      for (int i = 0; i < tab + 2; ++i) out << "\t";
      out << "col: (" << m_frames[f].col.r << ", " << m_frames[f].col.g
         << ", " << m_frames[f].col.b << ", " << m_frames[f].col.a << ")\n";

      if (m_frames[f].hasPoly) m_frames[f].poly.dbg_print(out, tab + 2);
   }
}
#endif

//===========================================
// Animation::Animation
//===========================================
Animation::Animation()
      : m_name(0), m_rate(0.f), m_state(STOPPED), m_frameReady(false) {}

//===========================================
// Animation::Animation
//===========================================
Animation::Animation(long name, float32_t rate, const std::vector<AnimFrame>& frames)
      : m_name(name), m_rate(rate), m_frames(frames), m_state(STOPPED), m_frameReady(false) {}

//===========================================
// Animation::assignData
//===========================================
void Animation::assignData(const xml_node<>* data) {
   if (strcmp(data->name(), "Animation") != 0)
      throw Exception("Error parsing XML for instance of class Animation", __FILE__, __LINE__);

   const xml_attribute<>* attr = data->first_attribute();
   if (attr && strcmp(attr->name(), "name") == 0) {
      m_name = internString(attr->value());
   }

   const xml_node<>* node = data->first_node();
   if (node && strcmp(node->name(), "rate") == 0) {
      sscanf(node->value(), "%f", &m_rate);
      node = node->next_sibling();
   }

   while (node) {
      if (strcmp(node->name(), "AnimFrame") == 0) {
         AnimFrame frame;
         frame.assignData(node);
         m_frames.push_back(frame);
      }

      node = node->next_sibling();
   }
}

//===========================================
// Animation::play
//===========================================
bool Animation::play() {
   switch (m_state) {
      case STOPPED:
         m_timer.reset();
         m_current = 0;
         m_state = PLAYING;
         m_prev = m_timer.getTime();
      break;
      case PAUSED:
         m_timer.reset();
         m_state = PLAYING;
         m_prev = m_timer.getTime();
      break;
      case PLAYING: return false;
   }

   return true;
}

//===========================================
// Animation::update
//===========================================
void Animation::update() {
   if (m_state != PLAYING) return;

   float32_t diff = m_timer.getTime() - m_prev;
   float32_t dt = 1.f / m_rate;
   if (diff > dt) {
      float32_t extra = diff - dt;

      do {
         m_frameReady = true;

         ++m_current;
         if (extra >= dt) extra -= dt;

         if (m_current == m_frames.size()) {
            m_state = STOPPED;
            break;
         }
      } while (extra >= dt);

      m_prev = m_timer.getTime() - extra;
   }
}


}
