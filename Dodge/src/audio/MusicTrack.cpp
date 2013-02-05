/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#include <StringId.hpp>
#include <audio/MusicTrack.hpp>
#include <audio/Audio.hpp>


using namespace std;


namespace Dodge {


//===========================================
// MusicTrack::MusicTrack
//===========================================
MusicTrack::MusicTrack(const XmlNode data)
   : Asset(internString("MusicTrack")) {

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
   audio.newMusicTrack(this, path);
}

//===========================================
// MusicTrack::MusicTrack
//===========================================
MusicTrack::MusicTrack(const string& file)
   : Asset(internString("MusicTrack")) {

   Audio audio;
   audio.newMusicTrack(this, file);
}

//===========================================
// MusicTrack::setVolume
//===========================================
void MusicTrack::setVolume(float vol) {
   Audio audio;
   audio.setMusicTrackVolume(shared_from_this(), vol);
}

//===========================================
// MusicTrack::play
//===========================================
void MusicTrack::play(bool repeat) {
   Audio audio;
   audio.playMusicTrack(shared_from_this(), repeat);
}

//===========================================
// MusicTrack::pause
//===========================================
void MusicTrack::pause() {
   Audio audio;
   audio.pauseMusicTrack(shared_from_this());
}

//===========================================
// MusicTrack::stop
//===========================================
void MusicTrack::stop() {
   Audio audio;
   audio.stopMusicTrack(shared_from_this());
}

//===========================================
// MusicTrack::clone
//===========================================
Asset* MusicTrack::clone() const {
   throw Exception("Cannot clone MusicTrack", __FILE__, __LINE__);
}

//===========================================
// MusicTrack::getSize
//===========================================
size_t MusicTrack::getSize() const {
   return 0; // TODO
}

//===========================================
// MusicTrack::~MusicTrack
//===========================================
MusicTrack::~MusicTrack() {
   Audio audio;
   audio.freeMusicTrack(this);
}


}
