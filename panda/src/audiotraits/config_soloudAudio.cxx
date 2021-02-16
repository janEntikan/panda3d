/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file config_soloudAudio.cxx
 * @author jan Entikan
 */

#include "pandabase.h"

#include "config_soloudAudio.h"
#include "soloudAudioManager.h"
#include "soloudAudioSound.h"
#include "pandaSystem.h"
#include "dconfig.h"

#if !defined(CPPPARSER) && !defined(LINK_ALL_STATIC) && !defined(BUILDING_SOLOUD_AUDIO)
  #error Buildsystem error: BUILDING_SOLOUD_AUDIO not defined
#endif

ConfigureDef(config_soloudAudio);
NotifyCategoryDef(soloudAudio, ":audio");

ConfigureFn(config_soloudAudio) {
  init_libSoLoudAudio();
}

ConfigVariableString soloud_device
("openal-device", "",
 PRC_DESC("Specify the SoLoud device string for audio playback (no quotes).  If this "
          "is not specified, the SoLoud default device is used."));

ConfigVariableInt soloud_buffer_delete_retries
("soloud-buffer-delete-retries", 5,
 PRC_DESC("If deleting a buffer fails due to still being in use, the SoLoud "
          "sound plugin will wait a moment and retry deletion, with an "
          "exponentially-increasing delay for each try.  This number "
          "specifies how many repeat tries (not counting the initial try) "
          "should be made before giving up and raising an error."));

ConfigVariableDouble soloud_buffer_delete_delay
("soloud-buffer-delete-delay", 0.001,
 PRC_DESC("If deleting a buffer fails due to still being in use, the SoLoud "
          "sound plugin will wait a moment and retry deletion, with an "
          "exponentially-increasing delay for each try.  This number "
          "specifies how long, in seconds, the OpenAL plugin will wait after "
          "its first failed try.  The second try will be double this "
          "delay, the third quadruple, and so on."));


/**
 * Initializes the library.  This must be called at least once before any of
 * the functions or classes in this library can be used.  Normally it will be
 * called by the static initializers and need not be called explicitly, but
 * special cases exist.
 */
void
init_libSoLoudAudio() {
  static bool initialized = false;
  if (initialized) {
    return;
  }

  initialized = true;
  SoLoudAudioManager::init_type();
  SoLoudAudioSound::init_type();

  AudioManager::register_AudioManager_creator(&Create_SoLoudAudioManager);

  PandaSystem *ps = PandaSystem::get_global_ptr();
  ps->add_system("SoLoud");
  ps->add_system("audio");
  ps->set_system_tag("audio", "implementation", "SoLoud");
}

/**
 * This function is called when the dynamic library is loaded; it should
 * return the Create_AudioManager function appropriate to create an
 * SoLoudAudioManager.
 */
Create_AudioManager_proc *
get_audio_manager_func_soloud_audio() {
  init_libSoLoudAudio();
  return &Create_SoLoudAudioManager;
}
