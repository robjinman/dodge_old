/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#include <StringId.hpp>
#include <audio/Audio.hpp>
#include <audio/Sound.hpp>


using namespace std;


namespace Dodge {


//===========================================
// Sound::Sound
//===========================================
Sound::Sound(const XmlNode data)
   : Asset(internString("Sound")) {

   string path;

   try {
      XML_NODE_CHECK(data, MusicTrack);

      XmlAttribute attr = data.firstAttribute();
      XML_ATTR_CHECK(attr, path);
      path = attr.getString();
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class MusicTrack; ");
      throw;
   }

   Audio audio;
   audio.newSound(this, path);
}

//===========================================
// Sound::Sound
//===========================================
Sound::Sound(const string& file)
   : Asset(internString("Sound")) {

   Audio audio;
   audio.newSound(this, file);
}

//===========================================
// Sound::setVolume
//===========================================
void Sound::setVolume(float vol) {
   Audio audio;
   audio.setSoundVolume(shared_from_this(), vol);
}

//===========================================
// Sound::play
//===========================================
void Sound::play() {
   Audio audio;
   audio.playSound(shared_from_this());
}

//===========================================
// Sound::stop
//===========================================
void Sound::stop() {
   Audio audio;
   audio.stopSound(shared_from_this());
}

//===========================================
// Sound::clone
//===========================================
Sound* Sound::clone() const {
   throw Exception("Cannot clone Sound", __FILE__, __LINE__);
}

//===========================================
// Sound::getSize
//===========================================
size_t Sound::getSize() const {
   return 0; // TODO
}

//===========================================
// Sound::~Sound
//===========================================
Sound::~Sound() {
   Audio audio;
   audio.freeSound(this);
}


}
