/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TRANSFORMATION_HPP__
#define __TRANSFORMATION_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include "rapidxml/rapidxml.hpp"
#include <boost/shared_ptr.hpp>
#include "math/Vec2f.hpp"
#include "Exception.hpp"
#include "Timer.hpp"
#include "TransFrame.hpp"
#include "Asset.hpp"


namespace Dodge {


class Entity;

class Transformation : public Asset {
   public:
      typedef enum { STOPPED, PLAYING, PAUSED } state_t;

      Transformation()
         : m_rate(1),
           m_current(0),
           m_smooth(1),
           m_state(STOPPED),
           m_frameReady(false),
           m_tmpFrame(Vec2f(0.0, 0.0), 0.0, Vec2f(1.0, 1.0)) {}

      Transformation(long name, double rate, const std::vector<TransFrame>& frames);

      // TODO
      virtual Transformation* clone() const { return NULL; }

      inline void setName(long name);
      inline void addFrame(const TransFrame& frame);
      inline void addFrames(const std::vector<TransFrame>& frames);
      inline void setSmooth(int smooth);
      inline void setFrameRate(double rate);

      inline long getName() const;
      inline int getSmooth() const;
      inline double getFrameRate() const;
      inline const TransFrame* getCurrentFrame();
      inline unsigned int getCurrentFrameIndex() const;
      inline unsigned int getNumFrames() const;
      inline state_t getState() const;

      inline void step();
      inline void setFrame(unsigned int frame);
      inline void stop();
      inline void pause();
      void play();
      void update();

      virtual void assignData(const rapidxml::xml_node<>* data);
#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

   private:
      Timer m_timer;
      long m_name;
      double m_rate;
      unsigned int m_current;
      double m_prev;
      int m_smooth;
      std::vector<TransFrame> m_frames;
      state_t m_state;
      bool m_frameReady;
      TransFrame m_tmpFrame;
};

typedef boost::shared_ptr<Transformation> pTransformation_t;

//===========================================
// Transformation::setName
//===========================================
inline void Transformation::setName(long name) {
   m_name = name;
}

//===========================================
// Transformation::addFrame
//===========================================
inline void Transformation::addFrame(const TransFrame& frame) {
   m_frames.push_back(frame);
}

//===========================================
// Transformation::addFrame
//===========================================
inline void Transformation::addFrames(const std::vector<TransFrame>& frames) {
   m_frames = frames;
}

//===========================================
// Transformation::setSmooth
//===========================================
inline void Transformation::setSmooth(int smooth) {
   m_smooth = smooth;
}

//===========================================
// Transformation::setFrameRate
//===========================================
inline void Transformation::setFrameRate(double rate) {
   m_rate = rate;
}

//===========================================
// Transformation::getName
//===========================================
inline long Transformation::getName() const {
   return m_name;
}

//===========================================
// Transformation::getSmooth
//===========================================
inline int Transformation::getSmooth() const {
   return m_smooth;
}

//===========================================
// Transformation::getFrameRate
//===========================================
inline double Transformation::getFrameRate() const {
   return m_rate;
}

//===========================================
// Transformation::getState
//===========================================
inline Transformation::state_t Transformation::getState() const {
   return m_state;
}

//===========================================
// Transformation::getCurrentFrameIndex
//===========================================
inline unsigned int Transformation::getCurrentFrameIndex() const {
   return m_current;
}

//===========================================
// Transformation::getNumFrames
//===========================================
inline unsigned int Transformation::getNumFrames() const {
   return m_frames.size();
}

//===========================================
// Transformation::getCurrentFrame
//===========================================
inline const TransFrame* Transformation::getCurrentFrame() {
   if (m_frameReady && m_current > 0) {
      m_frameReady = false;
      return &m_tmpFrame;
   }
   return NULL;
}

//===========================================
// Transformation::pause
//===========================================
inline void Transformation::pause() {
   m_state = PAUSED;
}

//===========================================
// Transformation::stop
//===========================================
inline void Transformation::stop() {
   m_state = STOPPED;
}

//===========================================
// Transformation::step
//===========================================
inline void Transformation::step() {
   ++m_current;
}

//===========================================
// Transformation::setFrame
//===========================================
inline void Transformation::setFrame(unsigned int frame) {
   if (frame < m_frames.size())
      m_current = frame;
   else
      throw Exception("Frame out of range", __FILE__, __LINE__);
}


}


#endif /*!__TRANSFORMATION_HPP__*/
