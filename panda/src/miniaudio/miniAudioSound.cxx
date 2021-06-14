/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file miniAudioSound.cxx
 * @author janentikan
 * @date 2021-06-14
 */

#include "pandabase.h"
#include "dcast.h"

// Panda Headers
#include "config_audio.h"
#include "config_miniAudio.h"
#include "miniAudioSound.h"
#include "string_utils.h"
#include "subfileInfo.h"
#include "reMutexHolder.h"
#include "virtualFileSystem.h"
#include "vector_uchar.h"

using std::istream;
using std::string;

TypeHandle MiniaudioSound::_type_handle;

/**
 * Constructor All sound will DEFAULT load as a 2D sound unless otherwise
 * specified.
 */

MiniaudioSound::
MiniaudioSound(AudioManager *manager, VirtualFile *file, bool positional) {
  ReMutexHolder holder(MiniaudioManager::_lock);
  audio_debug("MiniaudioSound::MiniaudioSound() Creating new sound, filename: "
              << file->get_original_filename());

  _active = manager->get_active();
  _paused = false;
  _start_time = 0.0;

  // Inits 3D Attributes
  // Play Rate Variable
  _playrate = 1;

  // Assign the values we need
  MiniaudioManager *fmanager;
  DCAST_INTO_V(fmanager, manager);
  _manager = fmanager;

  _file_name = file->get_original_filename();
  _file_name.set_binary();
}


/**
 * DESTRUCTOR!!!
 */
MiniaudioSound::
~MiniaudioSound() {
  ReMutexHolder holder(MiniaudioManager::_lock);

  // Remove me from table of all sounds.
  _manager->_all_sounds.erase(this);

  // The Release Sound
}


/**
 * Plays a sound.
 */
void MiniaudioSound::
play() {
  start_playing();
}

/**
 * Stop a sound
 */
void MiniaudioSound::
stop() {
  ReMutexHolder holder(MiniaudioManager::_lock);
  _start_time = 0.0;
  _paused = false;
}


/**
 * Turns looping on and off
 */
void MiniaudioSound::
set_loop(bool loop) {
  if (loop) {
    set_loop_count(0);
  } else {
    set_loop_count(1);
  }
}

/**
 * Returns whether looping is on or off
 */
bool MiniaudioSound::
get_loop() const {
  if (get_loop_count() == 1) {
    return false;
  } else {
    return true;
  }
}

/**
 *
 * Panda uses 0 to mean loop forever.  Fmod uses negative numbers to mean loop
 * forever.  (0 means don't loop, 1 means play twice, etc.  We must convert!
 */
void MiniaudioSound::
set_loop_count(unsigned long loop_count) {
  ReMutexHolder holder(MiniaudioManager::_lock);
}

/**
 * Return how many times a sound will loop.
 */
unsigned long MiniaudioSound::
get_loop_count() const {
  ReMutexHolder holder(MiniaudioManager::_lock);
}

/**
 * Sets the time at which the next play() operation will begin.  If we are
 * already playing, skips to that time immediatey.
 */
void MiniaudioSound::
set_time(PN_stdfloat start_time) {
  ReMutexHolder holder(MiniaudioManager::_lock);
  _start_time = start_time;

  if (status() == PLAYING) {
    // Already playing; skip to the indicated time.
    start_playing();
  }
}

/**
 * Gets the play position within the sound
 */
PN_stdfloat MiniaudioSound::
get_time() const {
  ReMutexHolder holder(MiniaudioManager::_lock);
  return (double)10;
}

/**
 * 0.0 to 1.0 scale of volume converted to Fmod's internal 0.0 to 255.0 scale.
 */
void MiniaudioSound::
set_volume(PN_stdfloat vol) {
  ReMutexHolder holder(MiniaudioManager::_lock);
  _volume = vol;
  set_volume_on_channel();
}

/**
 * Gets the current volume of a sound.  1 is Max.  O is Min.
 */
PN_stdfloat MiniaudioSound::
get_volume() const {
  return _volume;
}

/**
 * Starts the sound playing at _start_time.
 */
void MiniaudioSound::
start_playing() {
  ReMutexHolder holder(MiniaudioManager::_lock);
}

/**
 * Set the volume on a prepared Sound channel.
 */
void MiniaudioSound::
set_volume_on_channel() {
  ReMutexHolder holder(MiniaudioManager::_lock);
}

/**
 * -1.0 to 1.0 scale
 */
void MiniaudioSound::
set_balance(PN_stdfloat bal) {
  ReMutexHolder holder(MiniaudioManager::_lock);
  _balance = bal;
  set_speaker_mix_or_balance_on_channel();
}

/**
 * -1.0 to 1.0 scale -1 should be all the way left.  1 is all the way to the
 * right.
 */
PN_stdfloat MiniaudioSound::
get_balance() const {
  return _balance;
}

/**
 * Sets the speed at which a sound plays back.  The rate is a multiple of the
 * sound, normal playback speed.  IE 2 would play back 2 times fast, 3 would
 * play 3 times, and so on.  This can also be set to a negative number so a
 * sound plays backwards.  But rememeber if the sound is not playing, you must
 * set the sound's time to its end to hear a song play backwards.
 */
void MiniaudioSound::
set_play_rate(PN_stdfloat rate) {
  ReMutexHolder holder(MiniaudioManager::_lock);
  _playrate = rate;
  set_play_rate_on_channel();
}

/**
 *
 */
PN_stdfloat MiniaudioSound::
get_play_rate() const {
  return _playrate;
}

/**
 * Set the play rate on a prepared Sound channel.
 */
void MiniaudioSound::
set_play_rate_on_channel() {
  ReMutexHolder holder(MiniaudioManager::_lock);
}

/**
 * Get name of sound file
 */
const string& MiniaudioSound::
get_name() const {
  return _file_name;
}

/**
 * Get length MINIAUDIO returns the time in MS  so we have to convert to seconds.
 */
PN_stdfloat MiniaudioSound::
length() const {
  ReMutexHolder holder(MiniaudioManager::_lock);
  return ((double)10) / 1000.0;
}

void MiniaudioSound::
set_3d_attributes(PN_stdfloat px, PN_stdfloat py, PN_stdfloat pz, PN_stdfloat vx, PN_stdfloat vy, PN_stdfloat vz) {
  ReMutexHolder holder(MiniaudioManager::_lock);
  set_3d_attributes_on_channel();
}

/**
 *
 */
void MiniaudioSound::
set_3d_attributes_on_channel() {
  ReMutexHolder holder(MiniaudioManager::_lock);
}

/**
 * Get position and velocity of this sound Currently unimplemented.  Get the
 * attributes of the attached object.
 */
void MiniaudioSound::
get_3d_attributes(PN_stdfloat *px, PN_stdfloat *py, PN_stdfloat *pz, PN_stdfloat *vx, PN_stdfloat *vy, PN_stdfloat *vz) {
}

/**
 * Set the distance that this sound begins to fall off.  Also affects the rate
 * it falls off.
 */
void MiniaudioSound::
set_3d_min_distance(PN_stdfloat dist) {
  ReMutexHolder holder(MiniaudioManager::_lock);
}

/**
 * Get the distance that this sound begins to fall off
 */
PN_stdfloat MiniaudioSound::
get_3d_min_distance() const {
  return _min_dist;
}

/**
 * Set the distance that this sound stops falling off
 */
void MiniaudioSound::
set_3d_max_distance(PN_stdfloat dist) {
  ReMutexHolder holder(MiniaudioManager::_lock);
}

/**
 * Get the distance that this sound stops falling off
 */
PN_stdfloat MiniaudioSound::
get_3d_max_distance() const {
  return _max_dist;
}

/**
 * This is simply a safety catch.  If you are using a Stero speaker setup
 * Panda will only pay attention to 'set_balance()' command when setting
 * speaker balances.  Other wise it will use 'set_speaker_mix'. I put this in,
 * because other wise you end up with a sitation, where 'set_speaker_mix()' or
 * 'set_balace()' will override any previous speaker balance setups.  It all
 * depends on which was called last.
 */
void MiniaudioSound::
set_speaker_mix_or_balance_on_channel() {
  ReMutexHolder holder(MiniaudioManager::_lock);
}

/**
 * Sets the priority of a sound.  This is what MINIAUDIO uses to determine is a
 * sound will play if all the other real channels have been used up.
 */
int MiniaudioSound::
get_priority() {
  return _priority;
}

/**
 * Sets the Sound Priority [Whether is will be played over other sound when
 * real audio channels become short.
 */
void MiniaudioSound::
set_priority(int priority) {
  ReMutexHolder holder(MiniaudioManager::_lock);
  _priority = priority;
}

/**
 * Get status of the sound.
 */
AudioSound::SoundStatus MiniaudioSound::
status() const {
  ReMutexHolder holder(MiniaudioManager::_lock);
}

/**
 * Sets whether the sound is marked "active".  By default, the active flag
 * true for all sounds.  If the active flag is set to false for any particular
 * sound, the sound will not be heard.
 */
void MiniaudioSound::
set_active(bool active) {
  ReMutexHolder holder(MiniaudioManager::_lock);
}


/**
 * Returns whether the sound has been marked "active".
 */
bool MiniaudioSound::
get_active() const {
  return _active;
}

/**
 * Not implemented.
 */
void MiniaudioSound::
finished() {
}

/**
 * NOT USED ANYMORE!!! Assign a string for the finished event to be referenced
 * by in python by an accept method
 *
 */
void MiniaudioSound::
set_finished_event(const string& event) {
}

/**
 * NOT USED ANYMORE!!! Return the string the finished event is referenced by
 *

 *
 */
const string& MiniaudioSound::
get_finished_event() const {
  return _finished_event;
}
  // We can't yield here, since this callback is made within a sub-thread--an
  // OS-level sub-thread spawned by FMod, not a Panda thread.  But we will
  // only execute this code in the true-threads case anyway.
  // thread_consider_yield();
