/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <Animation.hpp>
#include <StringId.hpp>


using namespace std;


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
      out << "frame " << f << ":\n";

      m_frames[f].dbg_print(out, tab + 1);
   }
}
#endif

//===========================================
// Animation::Animation
//===========================================
Animation::Animation(const XmlNode data)
   : m_state(STOPPED), m_frameReady(false) {

   try {
      XML_NODE_CHECK(data, Animation);

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, name);
      m_name = internString(attr.getString());

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, rate);
      m_rate = attr.getFloat();

      XmlNode node = data.firstChild();
      while (!node.isNull() && node.name() == "AnimFrame") {
         AnimFrame frame(node);
         m_frames.push_back(frame);

         node = node.nextSibling();
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Animation; ");
      throw;
   }
}

//===========================================
// Animation::Animation
//===========================================
Animation::Animation(const Animation& copy, long name)
   : m_name(name), m_state(STOPPED), m_frameReady(false) {

   m_rate = copy.m_rate;
   m_frames = copy.m_frames;
}

//===========================================
// Animation::Animation
//===========================================
Animation::Animation(long name, float32_t rate, const std::vector<AnimFrame>& frames)
   : m_name(name), m_rate(rate), m_frames(frames), m_state(STOPPED), m_frameReady(false) {}

//===========================================
// Animation::clone
//===========================================
Animation* Animation::clone() const {
   return new Animation(*this);
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
   if (m_frames.empty()) return;

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
