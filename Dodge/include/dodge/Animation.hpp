/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include <vector>
#include <boost/shared_ptr.hpp>
#include "definitions.hpp"
#include "xml/xml.hpp"
#include "Exception.hpp"
#include "AnimFrame.hpp"
#include "Asset.hpp"


namespace Dodge {


class Animation : virtual public Asset {
   public:
      typedef enum { STOPPED, PLAYING, PAUSED } state_t;

      explicit Animation(const XmlNode data);
      Animation(const Animation& copy, long name);
      Animation(long name, float32_t rate, const std::vector<AnimFrame>& frames);

      virtual size_t getSize() const;
      virtual Animation* clone() const;

      inline void addFrame(const AnimFrame& frame);
      inline void setDuration(float32_t duration);

      inline long getName() const;
      inline float32_t getDuration() const;
      inline state_t getState() const;
      const AnimFrame* getCurrentFrame();
      inline uint_t getCurrentFrameIndex() const;
      inline const AnimFrame* getFrame(uint_t idx) const;
      inline uint_t getNumFrames() const;

      void step(bool* justFinished = NULL);
      inline void setFrame(uint_t frame);
      bool play(bool repeat = false);
      inline void pause();
      inline void stop();
      void update(bool* justFinished = NULL);

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

   private:
      long m_name;
      float32_t m_duration;
      uint_t m_frame;
      uint_t m_tickCount;
      std::vector<AnimFrame> m_frames;
      state_t m_state;
      bool m_repeat;

      bool m_frameReady;   // Indicates when frame is ready to be read via getCurrentFrame()
};

typedef boost::shared_ptr<Animation> pAnimation_t; // TODO: use unique_ptr instead

//===========================================
// Animation::addFrame
//===========================================
inline void Animation::addFrame(const AnimFrame& frame) {
   m_frames.push_back(frame);
   m_frames.back().number = m_frames.size() - 1;
}

//===========================================
// Animation::setDuration
//===========================================
inline void Animation::setDuration(float32_t duration) {
   m_duration = duration;
}

//===========================================
// Animation::getName
//===========================================
inline long Animation::getName() const {
   return m_name;
}

//===========================================
// Animation::getDuration
//===========================================
inline float32_t Animation::getDuration() const {
   return m_duration;
}

//===========================================
// Animation::getState
//===========================================
inline Animation::state_t Animation::getState() const {
   return m_state;
}

//===========================================
// Animation::getCurrentFrameIndex
//===========================================
inline uint_t Animation::getCurrentFrameIndex() const {
   return m_frame;
}

//===========================================
// Animation::getFrame
//===========================================
inline const AnimFrame* Animation::getFrame(uint_t idx) const {
   if (idx < m_frames.size())
      return &m_frames[idx];
   else
      throw Exception("Error retrieving animFrame; index out of range", __FILE__, __LINE__);
}

//===========================================
// Animation::getNumFrames
//===========================================
inline uint_t Animation::getNumFrames() const {
   return m_frames.size();
}

//===========================================
// Animation::setFrame
//===========================================
inline void Animation::setFrame(uint_t frame) {
   if (frame < m_frames.size())
      m_frame = frame;
   else
      throw Exception("Error setting animFrame; index out of range", __FILE__, __LINE__);
}

//===========================================
// Animation::pause
//===========================================
inline void Animation::pause() {
   m_state = PAUSED;
}

//===========================================
// Animation::stop
//===========================================
inline void Animation::stop() {
   m_state = STOPPED;
}


}


#endif /*!__ANIMATION_HPP__*/
