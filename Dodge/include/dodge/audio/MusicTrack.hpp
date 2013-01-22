/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __MUSIC_TRACK_HPP__
#define __MUSIC_TRACK_HPP__


#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <string>
#include "../xml/xml.hpp"
#include "../Asset.hpp"


namespace Dodge {


class MusicTrack : virtual public Asset, public boost::enable_shared_from_this<MusicTrack> {
   friend class Audio;

   public:
      MusicTrack(const XmlNode data);
      MusicTrack(const std::string& file);

      void setVolume(float vol);
      inline float getVolume() const;

      void play(bool repeat);
      void pause();
      void stop();

      virtual MusicTrack* clone() const;
      virtual size_t getSize() const;

      virtual ~MusicTrack();

   private:
      Mix_Music* m_data;
      float m_volume;
};

typedef boost::shared_ptr<MusicTrack> pMusicTrack_t;

//===========================================
// MusicTrack::getVolume
//===========================================
float MusicTrack::getVolume() const {
   return m_volume;
}


}


#endif
