/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2012
 */

#ifndef __TRANSFORMATION_HPP__
#define __TRANSFORMATION_HPP__


#ifdef DEBUG
#include <ostream>
#endif
#include <vector>
#include "xml/xml.hpp"
#include <boost/shared_ptr.hpp>
#include "math/Vec2f.hpp"
#include "Exception.hpp"
#include "TransPart.hpp"
#include "Asset.hpp"
#include "EEvent.hpp"
#include "StringId.hpp"


namespace Dodge {


class Entity;
class Transformation;

class ETransFinished : public EEvent {
   public:
      ETransFinished(boost::shared_ptr<Entity> entity_, const boost::shared_ptr<Transformation> trans_)
         : EEvent(internString("transFinished")), transformation(trans_), entity(entity_) {}

      const boost::shared_ptr<Transformation> transformation;
      boost::shared_ptr<Entity> entity;
};

class ETransPartFinished : public EEvent {
   public:
      ETransPartFinished(boost::shared_ptr<Entity> entity_, const boost::shared_ptr<Transformation> trans_)
         : EEvent(internString("transPartFinished")), transformation(trans_), entity(entity_) {}

      const boost::shared_ptr<Transformation> transformation;
      boost::shared_ptr<Entity> entity;
};

class Transformation : virtual public Asset {
   public:
      typedef enum { STOPPED, PLAYING, PAUSED } state_t;

      struct delta_t {
         delta_t()
            : transl(0, 0), rot(0), pivot(0, 0), scale(0, 0) {}

         delta_t(const Vec2f& t, float32_t r, const Vec2f& p, const Vec2f& s)
            : transl(t), rot(r), pivot(p), scale(s) {}

         Vec2f transl;
         float32_t rot;
         Vec2f pivot;
         Vec2f scale;
      };

      explicit Transformation(const XmlNode data);
      Transformation(const Transformation& copy, long name);
      Transformation(long name, const std::vector<TransPart>& parts);

      virtual size_t getSize() const;
      virtual Asset* clone() const;

      inline void setName(long name);
      void addPart(const TransPart& part);
      void addParts(const std::vector<TransPart>& parts);

      inline long getName() const;

      inline uint_t getCurrentPartIdx() const;
      inline uint_t getNumParts() const;
      inline uint_t getCurrentFrameNumber() const;
      inline uint_t getNumFrames() const;
      inline float32_t getDuration() const;

      inline state_t getState() const;

//      inline void setPart(uint_t part); // TODO
//      void setFrame(uint_t frame); // TODO
      inline void stop();
      inline void pause();
      void play();
      const delta_t* update();

#ifdef DEBUG
      virtual void dbg_print(std::ostream& out, int tab = 0) const;
#endif

   private:
      void init();

      long m_name;
      uint_t m_frame;
      uint_t m_numFrames;
      uint_t m_part;
      std::vector<TransPart> m_parts;
      state_t m_state;
      float32_t m_duration;
      delta_t m_delta;
      uint_t m_endOfPart;
};

typedef boost::shared_ptr<Transformation> pTransformation_t;

//===========================================
// Transformation::setName
//===========================================
inline void Transformation::setName(long name) {
   m_name = name;
}

//===========================================
// Transformation::getName
//===========================================
inline long Transformation::getName() const {
   return m_name;
}

//===========================================
// Transformation::getState
//===========================================
inline Transformation::state_t Transformation::getState() const {
   return m_state;
}

//===========================================
// Transformation::getCurrentPartIdx
//===========================================
inline uint_t Transformation::getCurrentPartIdx() const {
   return m_part;
}

//===========================================
// Transformation::getCurrentFrameNumber
//===========================================
inline uint_t Transformation::getCurrentFrameNumber() const {
   return m_frame;
}

//===========================================
// Transformation::getNumFrames
//===========================================
inline uint_t Transformation::getNumFrames() const {
   return m_numFrames;
}

//===========================================
// Transformation::getNumParts
//===========================================
inline uint_t Transformation::getNumParts() const {
   return m_parts.size();
}

//===========================================
// Transformation::getDuration
//===========================================
inline float32_t Transformation::getDuration() const {
   return m_duration;
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


}


#endif /*!__TRANSFORMATION_HPP__*/
