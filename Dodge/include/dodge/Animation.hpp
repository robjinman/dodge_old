/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __ANIMATION_HPP__
#define __ANIMATION_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include "definitions.hpp"
#include "rapidxml/rapidxml.hpp"
#include "Exception.hpp"
#include "AnimFrame.hpp"
#include "Timer.hpp"


namespace Dodge {


class Animation {
   public:
      typedef enum { STOPPED, PLAYING, PAUSED } state_t;

      Animation();
      Animation(long name, float32_t rate, const std::vector<AnimFrame>& frames);

      inline void addFrame(const AnimFrame& frame);
      inline void addFrames(const std::vector<AnimFrame>& frame);
      inline void setFrameRate(float32_t rate);

      inline long getName() const;
      inline float32_t getFrameRate() const;
      inline state_t getState() const;
      inline const AnimFrame* getCurrentFrame();
      inline uint_t getCurrentFrameIndex() const;
      inline const AnimFrame* getFrame(uint_t idx) const;
      inline uint_t getNumFrames() const;

      inline void step();
      inline void setFrame(uint_t frame);
      bool play();
      inline void pause();
      inline void stop();
      void update();

      virtual void assignData(const rapidxml::xml_node<>* data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

   private:
      Timer m_timer;
      long m_name;
      float32_t m_rate;
      uint_t m_current;
      float32_t m_prev;
      std::vector<AnimFrame> m_frames;
      state_t m_state;

      bool m_frameReady;   // Indicates when frame is ready to be read via getCurrentFrame()
};

//===========================================
// Animation::addFrame
//===========================================
inline void Animation::addFrame(const AnimFrame& frame) {
   m_frames.push_back(frame);
}

//===========================================
// Animation::addFrames
//===========================================
inline void Animation::addFrames(const std::vector<AnimFrame>& frames) {
   m_frames = frames;
}

//===========================================
// Animation::setFrameRate
//===========================================
inline void Animation::setFrameRate(float32_t rate) {
   m_rate = rate;
}

//===========================================
// Animation::getName
//===========================================
inline long Animation::getName() const {
   return m_name;
}

//===========================================
// Animation::getFrameRate
//===========================================
inline float32_t Animation::getFrameRate() const {
   return m_rate;
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
   return m_current;
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
// Animation::getCurrentFrame
//===========================================
inline const AnimFrame* Animation::getCurrentFrame() {
   if (m_frameReady && m_current > 0) {
      m_frameReady = false;
      return &m_frames[m_current - 1];
   }
   return NULL;
}

//===========================================
// Animation::setFrame
//===========================================
inline void Animation::setFrame(uint_t frame) {
   if (frame < m_frames.size())
      m_current = frame;
   else
      throw Exception("Error setting animFrame; index out of range", __FILE__, __LINE__);
}

//===========================================
// Animation::step
//===========================================
inline void Animation::step() {
   ++m_current;
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
