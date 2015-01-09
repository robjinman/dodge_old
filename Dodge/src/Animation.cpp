/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#include <cstring>
#include <Animation.hpp>
#include <StringId.hpp>
#include <globals.hpp>


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
   out << "duration: " << m_duration << "\n";

   for (unsigned int f = 0; f < m_frames.size(); ++f) {
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
   : Asset(internString("Animation")),
     m_state(STOPPED),
     m_frameReady(false) {

   try {
      XML_NODE_CHECK(data, Animation);

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, name);
      m_name = internString(attr.getString());

      attr = attr.nextAttribute();
      XML_ATTR_CHECK(attr, duration);
      m_duration = attr.getFloat();

      uint_t f = 0;
      XmlNode node = data.firstChild();
      while (!node.isNull() && node.name() == "AnimFrame") {
         AnimFrame frame(node);
         frame.number = f;

         m_frames.push_back(frame);

         ++f;
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
   : Asset(internString("Animation")),
     m_name(name),
     m_state(STOPPED),
     m_frameReady(false) {

   m_duration = copy.m_duration;
   m_frames = copy.m_frames;
}

//===========================================
// Animation::Animation
//===========================================
Animation::Animation(long name, float32_t duration, const std::vector<AnimFrame>& frames)
   : Asset(internString("Animation")),
     m_name(name),
     m_duration(duration),
     m_frames(frames),
     m_state(STOPPED),
     m_frameReady(false) {

   for (uint_t i = 0; i < m_frames.size(); ++i)
      m_frames[i].number = i;
}

//===========================================
// Animation::clone
//===========================================
Asset* Animation::clone() const {
   return new Animation(*this);
}

//===========================================
// Animation::getSize
//===========================================
size_t Animation::getSize() const {

   size_t framesSz = 0;
   for (uint_t i = 0; i < m_frames.size(); ++i)
      framesSz += m_frames[i].getSize();

   return sizeof(Animation) + framesSz;
}

//===========================================
// Animation::play
//===========================================
bool Animation::play(bool repeat) {
   switch (m_state) {
      case STOPPED:
         m_frame = 0;
         m_tickCount = 0;
         m_frameReady = true;
         m_state = PLAYING;
         m_repeat = repeat;
      break;
      case PAUSED:
         m_state = PLAYING;
         m_repeat = repeat;
      break;
      case PLAYING: return false;
   }

   return true;
}

//===========================================
// Animation::step
//===========================================
void Animation::step(bool* justFinished) {
   if (m_state == PAUSED) {
      ++m_frame;

      if (m_frame < m_frames.size()) {
         m_frameReady = true;
      }
      else {
         if (m_repeat) {
            m_frame = 0;
            m_frameReady = true;
         }
         else {
            m_state = STOPPED;
            m_frameReady = false;
         }

         if (justFinished) *justFinished = true;
      }
   }
}

//===========================================
// Animation::getCurrentFrame
//===========================================
const AnimFrame* Animation::getCurrentFrame() {
   if (m_frameReady) {
      m_frameReady = false;
      return &m_frames[m_frame];
   }
   return NULL;
}

//===========================================
// Animation::update
//===========================================
void Animation::update(bool* justFinished) {
   if (m_state != PLAYING) return;
   if (m_frames.empty()) return;

   ++m_tickCount;

   uint_t n = static_cast<uint_t>((gGetTargetFrameRate() * m_duration) / static_cast<float32_t>(m_frames.size()));

   if (m_tickCount >= n) {
      ++m_frame;

      if (m_frame < m_frames.size()) {
         m_tickCount = 0;
         m_frameReady = true;
      }
      else {
         if (m_repeat) {
            m_frame = 0;
            m_tickCount = 0;
            m_frameReady = true;
         }
         else {
            m_state = STOPPED;
            m_frameReady = false;
         }

         if (justFinished) *justFinished = true;
      }
   }
}


}
