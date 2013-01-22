/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#ifndef __AUDIO_HPP__
#define __AUDIO_HPP__


#include <string>
#include <SDL_mixer.h>
#include "../definitions.hpp"
#include "Sound.hpp"
#include "MusicTrack.hpp"


namespace Dodge {


// Note this limitation: adjusting the master sound fx volume via setSoundVolume()
// does not affect sounds while they currently playing; only subsequent plays are
// affected.
class Audio {
   friend class Sound;
   friend class MusicTrack;

   public:
      void initialise();
      void quit();

      void setMasterVolume(float32_t vol);
      void setSoundVolume(float32_t vol);
      void setMusicVolume(float32_t vol);

      void playMusic(bool rep);
      void pauseMusic();
      void stopMusic();

   private:
      void newSound(Sound* sound, const std::string& file);
      void freeSound(Sound* sound);
      void playSound(pSound_t sound);
      void stopSound(pSound_t sound);

      void setSoundVolume(pSound_t sound, float32_t vol);
      void setMusicTrackVolume(pMusicTrack_t track, float32_t vol);

      void adjustSdlMusicVol();
      void adjustSdlSfxVol(pSound_t sound);

      void newMusicTrack(MusicTrack* track, const std::string& file);
      void freeMusicTrack(MusicTrack* track);
      void playMusicTrack(pMusicTrack_t track, bool repeat);
      void pauseMusicTrack(pMusicTrack_t track);
      void stopMusicTrack(pMusicTrack_t track);

      static float32_t m_masterVol;
      static float32_t m_soundVol;
      static float32_t m_musicVol;
      static pMusicTrack_t m_music;
};


}


#endif
