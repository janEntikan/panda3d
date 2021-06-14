/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file miniAudioManager.cxx
 * @author janentikan
 * @date 2021-06-14
 */

#include "pandabase.h"
#include "config_audio.h"
#include "config_miniAudio.h"
#include "dcast.h"

// Panda headers.
#include "config_audio.h"
#include "config_putil.h"
#include "miniAudioManager.h"
#include "miniAudioSound.h"
#include "filename.h"
#include "virtualFileSystem.h"
#include "reMutexHolder.h"

// MINIAUDIO Headers.
#include "miniaudio.h"
#include "miniaudio_experimental.h"
#include "miniaudio_reverb.h"


TypeHandle MiniaudioManager::_type_handle;

ReMutex MiniaudioManager::_lock;

pset<MiniaudioManager *> MiniaudioManager::_all_managers;

bool MiniaudioManager::_system_is_valid = false;

PN_stdfloat MiniaudioManager::_doppler_factor = 1;
PN_stdfloat MiniaudioManager::_distance_factor = 1;
PN_stdfloat MiniaudioManager::_drop_off_factor = 1;


// Central dispatcher for audio errors.

void miniaudio_audio_errcheck(const char *context) {
}

/**
 * Factory Function
 */
AudioManager *Create_MiniaudioManager() {
  audio_debug("Create_MiniaudioManager()");
  return new MiniaudioManager;
}


/**
 *
 */
MiniaudioManager::
MiniaudioManager() {
  ReMutexHolder holder(_lock);
  _all_managers.insert(this);

  // TODO: Init 3D attributes
    // TODO: Initialize the System.
    // TODO: Initialze 3d audio
}

/**
 *
 */
MiniaudioManager::
~MiniaudioManager() {
  ReMutexHolder holder(_lock);
  // Be sure to delete associated sounds before deleting the manager!
  // Release Sounds Next
  _all_sounds.clear();
  // Remove me from the managers list.
  _all_managers.erase(this);
  // TODO: release system
}

/**
 * This just check to make sure the MINIAUDIO System is up and running correctly.
 */
bool MiniaudioManager::
is_valid() {
  return _is_valid;
}

/**
 * TODO: MINIAUDIO_effects
 */

/**
 * This is what creates a sound instance.
 */
PT(AudioSound) MiniaudioManager::
get_sound(const Filename &file_name, bool positional, int) {
  ReMutexHolder holder(_lock);
  // Needed so People use Panda's Generic UNIX Style Paths for Filename.
  // path.to_os_specific() converts it back to the proper OS version later on.

  Filename path = file_name;

  VirtualFileSystem *vfs = VirtualFileSystem::get_global_ptr();
  vfs->resolve_filename(path, get_model_path());

  // Locate the file on disk.
  path.set_binary();
  PT(VirtualFile) file = vfs->get_file(path);
  if (file != nullptr) {
    // Build a new AudioSound from the audio data.
    PT(MiniaudioSound) sound = new MiniaudioSound(this, file, positional);

    _all_sounds.insert(sound);
    return sound;
  } else {
    audio_error("createSound(" << path << "): File not found.");
    return get_null_sound();
  }
}

/**
 * This is what creates a sound instance.
 */
PT(AudioSound) MiniaudioManager::
get_sound(MovieAudio *source, bool positional, int) {
  nassert_raise("MINIAUDIO audio manager does not support MovieAudio sources");
  return nullptr;
}

/**
 * This is to query if you are using a MultiChannel Setup.
 */
int MiniaudioManager::
get_speaker_setup() {
  ReMutexHolder holder(_lock);
  return 0;
}


void MiniaudioManager::
set_speaker_setup(AudioManager::SpeakerModeCategory cat) {
  ReMutexHolder holder(_lock);
}

/**
 * Sets the volume of the AudioManager.  It is not an override, but a
 * multiplier.
 */
void MiniaudioManager::
set_volume(PN_stdfloat volume) {
  ReMutexHolder holder(_lock);
}

/**
 * Returns the AudioManager's volume.
 */
PN_stdfloat MiniaudioManager::
get_volume() const {
  ReMutexHolder holder(_lock);
  float volume;
  return (PN_stdfloat)0;
}

/**
 * Changes output mode to write all audio to a wav file.
 */
void MiniaudioManager::
set_wavwriter(bool outputwav) {
  ReMutexHolder holder(_lock);
}


/**
 * Turn on/off Warning: not implemented.
 */
void MiniaudioManager::
set_active(bool active) {
  ReMutexHolder holder(_lock);
}

/**
 *
 */
bool MiniaudioManager::
get_active() const {
  return _active;
}

/**
 * Stop playback on all sounds managed by this manager.
 */
void MiniaudioManager::
stop_all_sounds() {
  ReMutexHolder holder(_lock);
}

/**
 * Perform all per-frame update functions.
 */
void MiniaudioManager::
update() {
  ReMutexHolder holder(_lock);
}

void MiniaudioManager::
audio_3d_set_listener_attributes(PN_stdfloat px, PN_stdfloat py, PN_stdfloat pz, PN_stdfloat vx, PN_stdfloat vy, PN_stdfloat vz, PN_stdfloat fx, PN_stdfloat fy, PN_stdfloat fz, PN_stdfloat ux, PN_stdfloat uy, PN_stdfloat uz) {
  ReMutexHolder holder(_lock);
}

/**
 * Get position of the "ear" that picks up 3d sounds
 */
void MiniaudioManager::
audio_3d_get_listener_attributes(PN_stdfloat *px, PN_stdfloat *py, PN_stdfloat *pz, PN_stdfloat *vx, PN_stdfloat *vy, PN_stdfloat *vz, PN_stdfloat *fx, PN_stdfloat *fy, PN_stdfloat *fz, PN_stdfloat *ux, PN_stdfloat *uy, PN_stdfloat *uz) {
}


/**
 * Set units per meter (Miniaudio uses meters internally for its sound-
 * spacialization calculations)
 */
void MiniaudioManager::
audio_3d_set_distance_factor(PN_stdfloat factor) {
  ReMutexHolder holder(_lock);
}

/**
 * Gets units per meter (Miniaudio uses meters internally for its sound-
 * spacialization calculations)
 */
PN_stdfloat MiniaudioManager::
audio_3d_get_distance_factor() const {
  return _distance_factor;
}

/**
 * Exaggerates or diminishes the Doppler effect.  Defaults to 1.0
 */
void MiniaudioManager::
audio_3d_set_doppler_factor(PN_stdfloat factor) {
  ReMutexHolder holder(_lock);

}

/**
 *
 */
PN_stdfloat MiniaudioManager::
audio_3d_get_doppler_factor() const {
  return _doppler_factor;
}

/**
 * Control the effect distance has on audability.  Defaults to 1.0
 */
void MiniaudioManager::
audio_3d_set_drop_off_factor(PN_stdfloat factor) {
  ReMutexHolder holder(_lock);
}

/**
 *
 */
PN_stdfloat MiniaudioManager::
audio_3d_get_drop_off_factor() const {
  ReMutexHolder holder(_lock);
  return _drop_off_factor;
}



/**
 * NOT USED FOR MINIAUDIO-EX!!!
 */
void MiniaudioManager::
set_concurrent_sound_limit(unsigned int limit) {

}

/**
 * NOT USED FOR MINIAUDIO-EX!!!
 */
unsigned int MiniaudioManager::
get_concurrent_sound_limit() const {
  return 1000000;
}

/**
 * NOT USED FOR MINIAUDIO-EX!!!
 */
void MiniaudioManager::
reduce_sounds_playing_to(unsigned int count) {

}


/**
 * NOT USED FOR MINIAUDIO-EX!!! Clears a sound out of the sound cache.
 */
void MiniaudioManager::
uncache_sound(const Filename &file_name) {
}


/**
 * NOT USED FOR MINIAUDIO-EX!!! Clear out the sound cache.
 */
void MiniaudioManager::
clear_cache() {
}

/**
 * NOT USED FOR MINIAUDIO-EX!!! Set the number of sounds that the cache can hold.
 */
void MiniaudioManager::
set_cache_limit(unsigned int count) {
}

/**
 * NOT USED FOR MINIAUDIO-EX!!! Gets the number of sounds that the cache can hold.
 */
unsigned int MiniaudioManager::
get_cache_limit() const {
  return 0;
}
