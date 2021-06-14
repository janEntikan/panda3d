/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @author janentikan
 * @date 2021-06-14
 */

#ifndef __MINIAUDIO_AUDIO_SOUND_H__
#define __MINIAUDIO_AUDIO_SOUND_H__

#include "pandabase.h"

#include "audioSound.h"
#include "reMutex.h"
#include "miniAudioManager.h"

#include "miniaudio.h"
#include "miniaudio_experimental.h"
#include "miniaudio_reverb.h"

class VirtualFile;

class MiniaudioSound : public AudioSound {
public:
  MiniaudioSound(AudioManager *manager, VirtualFile *file, bool positional);
  ~MiniaudioSound();

  // For best compatibility, set the loop_count, start_time, volume, and
  // balance, prior to calling play().  You may set them while they're
  // playing, but it's implementation specific whether you get the results.
  void play();
  void stop();

  // loop: false = play once; true = play forever.  inits to false.
  void set_loop(bool loop=true);
  bool get_loop() const;

  // loop_count: 0 = forever; 1 = play once; n = play n times.  inits to 1.
  void set_loop_count(unsigned long loop_count=1);
  unsigned long get_loop_count() const;

  // 0 = beginning; length() = end.  inits to 0.0.
  void set_time(PN_stdfloat start_time=0.0);
  PN_stdfloat get_time() const;

  // 0 = minimum; 1.0 = maximum.  inits to 1.0.
  void set_volume(PN_stdfloat volume=1.0);
  PN_stdfloat get_volume() const;

  // -1.0 is hard left 0.0 is centered 1.0 is hard right inits to 0.0.
  void set_balance(PN_stdfloat balance_right=0.0);
  PN_stdfloat get_balance() const;

  // play_rate is any positive float value.  inits to 1.0.
  void set_play_rate(PN_stdfloat play_rate=1.0f);
  PN_stdfloat get_play_rate() const;

  const std::string &get_name() const;

  // return: playing time in seconds.
  PN_stdfloat length() const;

  // Controls the position of this sound's emitter.  pos is a pointer to an
  // xyz triplet of the emitter's position.  vel is a pointer to an xyz
  // triplet of the emitter's velocity.
  void set_3d_attributes(PN_stdfloat px, PN_stdfloat py, PN_stdfloat pz, PN_stdfloat vx, PN_stdfloat vy, PN_stdfloat vz);
  void get_3d_attributes(PN_stdfloat *px, PN_stdfloat *py, PN_stdfloat *pz, PN_stdfloat *vx, PN_stdfloat *vy, PN_stdfloat *vz);

  void set_3d_min_distance(PN_stdfloat dist);
  PN_stdfloat get_3d_min_distance() const;

  void set_3d_max_distance(PN_stdfloat dist);
  PN_stdfloat get_3d_max_distance() const;

  AudioSound::SoundStatus status() const;

  virtual PN_stdfloat get_speaker_mix(int speaker);
  virtual void set_speaker_mix(PN_stdfloat frontleft, PN_stdfloat frontright, PN_stdfloat center, PN_stdfloat sub, PN_stdfloat backleft, PN_stdfloat backright, PN_stdfloat sideleft, PN_stdfloat  sideright);

  void set_active(bool active=true);
  bool get_active() const;

  void finished();
  void set_finished_event(const std::string& event);
  const std::string& get_finished_event() const;

 private:
  PT(MiniaudioManager) _manager;

  Filename _file_name;

  float _volume;
  float _balance;
  float _playrate;
  int   _priority;
  float _mix[AudioManager::SPK_COUNT];

  float _sampleFrequency;
  mutable float _length;   //in seconds.

  PN_stdfloat _min_dist;
  PN_stdfloat _max_dist;

  void start_playing();
  void set_volume_on_channel();
  void set_balance_on_channel();
  void set_play_rate_on_channel();
  void set_speaker_mix_on_channel();
  void set_3d_attributes_on_channel();
  // void add_dsp_on_channel();
  void set_speaker_mix_or_balance_on_channel();

  virtual int get_priority();
  virtual void set_priority(int priority);

  bool _active;
  bool _paused;
  PN_stdfloat _start_time;

  std::string _finished_event;

  // This reference-counting pointer is set to this while the sound is
  // playing, and cleared when we get an indication that the sound has
  // stopped.  This prevents a sound from destructing while it is playing.  We
  // use a PT instead of managing the reference counts by hand to help guard
  // against accidental reference count leaks or other mismanagement.
  PT(MiniaudioSound) _self_ref;

  // These are needed for Panda's Pointer System.  DO NOT ERASE!

 public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    AudioSound::init_type();
    register_type(_type_handle, "MiniaudioSound", AudioSound::get_class_type());
  }
  virtual TypeHandle get_type() const {
    return get_class_type();
  }
  virtual TypeHandle force_init_type() {
    init_type();
    return get_class_type();
  }

 private:
  static TypeHandle _type_handle;

  // DONE
};

#include "miniAudioSound.I"

#endif /* __MINIAUDIO_AUDIO_SOUND_H__ */
