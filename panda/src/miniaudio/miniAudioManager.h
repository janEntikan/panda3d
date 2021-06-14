/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file miniAudioManager.h
 * @author janentikan
 * @date 2021-06-14
 */

#ifndef MINIAUDIOAUDIOMANAGER_H
#define MINIAUDIOAUDIOMANAGER_H

// First the includes.
#include "pandabase.h"
#include "pset.h"

#include "audioManager.h"

#include "miniaudio.h"
#include "miniaudio_experimental.h"
#include "miniaudio_reverb.h"

class MiniaudioSound;

class MiniaudioManager : public AudioManager {
  friend class MiniaudioSound;

public:
  MiniaudioManager();
  virtual ~MiniaudioManager();

  virtual bool is_valid();

  virtual PT(AudioSound) get_sound(const Filename &, bool positional = false, int mode=SM_heuristic);
  virtual PT(AudioSound) get_sound(MovieAudio *,  bool positional = false, int mode=SM_heuristic);

  virtual int get_speaker_setup();
  virtual void set_speaker_setup(SpeakerModeCategory cat);

  virtual void set_volume(PN_stdfloat);
  virtual PN_stdfloat get_volume() const;

  virtual void set_wavwriter(bool);

  virtual void set_active(bool);
  virtual bool get_active() const;

  virtual void stop_all_sounds();

  virtual void update();

  // This controls the "set of ears" that listens to 3D spacialized sound px,
  // py, pz are position coordinates.  Can be 0.0f to ignore.  vx, vy, vz are
  // a velocity vector in UNITS PER SECOND (default: meters). fx, fy and fz
  // are the respective components of a unit forward-vector ux, uy and uz are
  // the respective components of a unit up-vector These changes will NOT be
  // invoked until audio_3d_update() is called.
  virtual void audio_3d_set_listener_attributes(PN_stdfloat px, PN_stdfloat py, PN_stdfloat pz,
                                                PN_stdfloat vx, PN_stdfloat xy, PN_stdfloat xz,
                                                PN_stdfloat fx, PN_stdfloat fy, PN_stdfloat fz,
                                                PN_stdfloat ux, PN_stdfloat uy, PN_stdfloat uz);

  // REMOVE THIS ONE
  virtual void audio_3d_get_listener_attributes(PN_stdfloat *px, PN_stdfloat *py, PN_stdfloat *pz,
                                                PN_stdfloat *vx, PN_stdfloat *vy, PN_stdfloat *vz,
                                                PN_stdfloat *fx, PN_stdfloat *fy, PN_stdfloat *fz,
                                                PN_stdfloat *ux, PN_stdfloat *uy, PN_stdfloat *uz);

  // Control the "relative scale that sets the distance factor" units for 3D
  // spacialized audio. This is a float in units-per-meter. Default value is
  // 1.0, which means that Panda units are understood as meters; for e.g.
  // feet, set 3.28. This factor is applied only to Fmod and OpenAL at the
  // moment.
  virtual void audio_3d_set_distance_factor(PN_stdfloat factor);
  virtual PN_stdfloat audio_3d_get_distance_factor() const;

  // Control the presence of the Doppler effect.  Default is 1.0 Exaggerated
  // Doppler, use >1.0 Diminshed Doppler, use <1.0
  virtual void audio_3d_set_doppler_factor(PN_stdfloat factor);
  virtual PN_stdfloat audio_3d_get_doppler_factor() const;

  // Exaggerate or diminish the effect of distance on sound.  Default is 1.0
  // Faster drop off, use >1.0 Slower drop off, use <1.0
  virtual void audio_3d_set_drop_off_factor(PN_stdfloat factor);
  virtual PN_stdfloat audio_3d_get_drop_off_factor() const;

  // THESE ARE NOT USED ANYMORE. THEY ARE ONLY HERE BECAUSE THEY are still
  // needed by Miles.  THESE are stubs in MINIAUDIO-EX version
  virtual void set_concurrent_sound_limit(unsigned int limit = 0);
  virtual unsigned int get_concurrent_sound_limit() const;
  virtual void reduce_sounds_playing_to(unsigned int count);
  virtual void uncache_sound(const Filename &);
  virtual void clear_cache();
  virtual void set_cache_limit(unsigned int count);
  virtual unsigned int get_cache_limit() const;

private:
  // This global lock protects all access to FMod library interfaces.
  static ReMutex _lock;
  static pset<MiniaudioManager *> _all_managers;

  static bool _system_is_valid;

  static PN_stdfloat _distance_factor;
  static PN_stdfloat _doppler_factor;
  static PN_stdfloat _drop_off_factor;

  bool _is_valid;
  bool _active;

  // The set of all sounds.  Needed only to implement stop_all_sounds.
  typedef pset<MiniaudioSound *> SoundSet;
  SoundSet _all_sounds;

public:
  static TypeHandle get_class_type() {
    return _type_handle;
  }
  static void init_type() {
    AudioManager::init_type();
    register_type(_type_handle, "MiniaudioManager", AudioManager::get_class_type());
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
};

AudioManager *Create_MiniaudioManager();


#endif /* MINIAUDIOAUDIOMANAGER_H */
