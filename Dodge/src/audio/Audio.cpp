/*
 * Author: Rob Jinman <admin@robjinman.com>
 * Date: 2013
 */

#include <SDL.h>
#include <audio/AudioException.hpp>
#include <audio/Audio.hpp>


using namespace std;


namespace Dodge {


pMusicTrack_t Audio::m_music;
float32_t Audio::m_masterVol = 1.0;
float32_t Audio::m_soundVol = 1.0;
float32_t Audio::m_musicVol = 1.0;


//===========================================
// Audio::initialise
//===========================================
void Audio::initialise() { // TODO: parameterise these options via a loadSettingsFromFile() method.
   SDL_Init(SDL_INIT_AUDIO);

   int audioRate = 44100;
   Uint16 audioFormat = AUDIO_S16; 
   int audioChannels = 2;
   int audioBuffers = 2048;

   if (Mix_OpenAudio(audioRate, audioFormat, audioChannels, audioBuffers))
      throw AudioException("Could not open audio", __FILE__, __LINE__);

   Mix_AllocateChannels(8);
}

//===========================================
// Audio::quit
//===========================================
void Audio::quit() {
   Mix_HaltChannel(-1);
   SDL_Quit();
}

//===========================================
// Audio::setMasterVolume
//===========================================
void Audio::setMasterVolume(float32_t vol) {
   m_masterVol = vol;
   adjustSdlMusicVol();
}

//===========================================
// Audio::playMusic
//===========================================
void Audio::playMusic(bool rep) {
   if (m_music) {
      if (!Mix_PlayingMusic()) Mix_PlayMusic(m_music->m_data, rep ? -1 : 0);
      if (Mix_PausedMusic()) Mix_ResumeMusic();
   }
}

//===========================================
// Audio::pauseMusic
//===========================================
void Audio::pauseMusic() {
   Mix_PauseMusic();
}

//===========================================
// Audio::stopMusic
//===========================================
void Audio::stopMusic() {
   Mix_HaltMusic();
}

//===========================================
// Audio::setMusicVolume
//===========================================
void Audio::setMusicVolume(float32_t vol) {
   m_musicVol = vol;
   adjustSdlMusicVol();
}

//===========================================
// Audio::adjustSdlMusicVol
//===========================================
void Audio::adjustSdlMusicVol() {
   if (m_music)
      Mix_VolumeMusic(static_cast<int>(m_music->m_volume * m_masterVol * m_musicVol * static_cast<float32_t>(MIX_MAX_VOLUME)));
}

//===========================================
// Audio::adjustSdlSfxVol
//===========================================
void Audio::adjustSdlSfxVol(pSound_t sound) {
   Mix_VolumeChunk(sound->m_chunk, static_cast<int>(sound->m_volume * m_soundVol * m_masterVol * static_cast<float32_t>(MIX_MAX_VOLUME)));
}

//===========================================
// Audio::setSoundVolume
//===========================================
void Audio::setSoundVolume(float32_t vol) {
   m_soundVol = vol;
}

//===========================================
// Audio::newSound
//===========================================
void Audio::newSound(Sound* sound, const string& file) {
   Mix_Chunk* chunk = Mix_LoadWAV(file.data());

   if (chunk == NULL)
      throw AudioException("Error loading sound from file", __FILE__, __LINE__);

   sound->m_chunk = chunk;
   sound->m_channel = -1;
   sound->m_volume = MIX_MAX_VOLUME;
}

//===========================================
// Audio::freeSound
//===========================================
void Audio::freeSound(Sound* sound) {
   Mix_FreeChunk(sound->m_chunk);
}

//===========================================
// Audio::playSound
//===========================================
void Audio::playSound(pSound_t sound) {
   adjustSdlSfxVol(sound);
   sound->m_channel = Mix_PlayChannel(-1, sound->m_chunk, 0);
}

//===========================================
// Audio::setSoundVolume
//===========================================
void Audio::setSoundVolume(pSound_t sound, float32_t vol) {
   sound->m_volume = vol;

// Not sure if this is safe if sound is currently playing
//   adjustSdlSfxVol(sound);
}

//===========================================
// Audio::stopSound
//===========================================
void Audio::stopSound(pSound_t sound) {
   if (sound->m_channel != -1)
      Mix_HaltChannel(sound->m_channel);
}

//===========================================
// Audio::newMusicTrack
//===========================================
void Audio::newMusicTrack(MusicTrack* track, const string& file) {
   track->m_data = Mix_LoadMUS(file.data());
   track->m_volume = MIX_MAX_VOLUME;
}

//===========================================
// Audio::freeMusicTrack
//===========================================
void Audio::freeMusicTrack(MusicTrack* track) {
   if (track == m_music.get()) stopMusic();

   if (track->m_data) {
      Mix_FreeMusic(track->m_data);
      track->m_data = NULL;
   }
}

//===========================================
// Audio::playMusicTrack
//===========================================
void Audio::playMusicTrack(pMusicTrack_t track, bool repeat) {
   if (track != m_music) {     // If another track is playing
      stopMusicTrack(m_music);
   }

   m_music = track;
   adjustSdlMusicVol();
   playMusic(repeat);
}

//===========================================
// Audio::pauseMusicTrack
//===========================================
void Audio::pauseMusicTrack(pMusicTrack_t track) {
   if (track == m_music) pauseMusic();
}

//===========================================
// Audio::stopMusicTrack
//===========================================
void Audio::stopMusicTrack(pMusicTrack_t track) {
   if (track == m_music) {
      stopMusic();
      m_music.reset();
   }
}

//===========================================
// Audio::setMusicTrackVolume
//===========================================
void Audio::setMusicTrackVolume(pMusicTrack_t track, float32_t vol) {
   track->m_volume = vol;
   if (track == m_music) adjustSdlMusicVol();
}


}
