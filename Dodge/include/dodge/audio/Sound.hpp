/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __SOUND_HPP__
#define __SOUND_HPP__


#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include "../xml/xml.hpp"
#include "../Asset.hpp"


namespace Dodge {


class Sound : virtual public Asset, public boost::enable_shared_from_this<Sound> {
   friend class Audio;

   public:
      Sound(const XmlNode data);
      Sound(const std::string& file);

      void setVolume(float vol);
      inline float getVolume() const;

      void play();
      void stop();

      virtual Sound* clone() const;
      virtual size_t getSize() const;

      virtual ~Sound();

   private:
      Mix_Chunk* m_chunk;
      int m_channel;
      float m_volume;
};

typedef boost::shared_ptr<Sound> pSound_t;

//===========================================
// Sound::getVolume
//===========================================
float Sound::getVolume() const {
   return m_volume;
}


}


#endif
