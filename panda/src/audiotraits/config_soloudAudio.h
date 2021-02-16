/**
 * PANDA 3D SOFTWARE
 * Copyright (c) Carnegie Mellon University.  All rights reserved.
 *
 * All use of this software is subject to the terms of the revised BSD
 * license.  You should have received a copy of this license along
 * with this source code in a file named "LICENSE."
 *
 * @file config_soloudAudio.h
 * @author jan Entikan
 */

#ifndef CONFIG_SOLOUDAUDIO_H
#define CONFIG_SOLOUDAUDIO_H

#include "pandabase.h"

#include "notifyCategoryProxy.h"
#include "dconfig.h"
#include "audioManager.h"

ConfigureDecl(config_soloudAudio, EXPCL_SOLOUD_AUDIO, EXPTP_SOLOUD_AUDIO);
NotifyCategoryDecl(soloudAudio, EXPCL_SOLOUD_AUDIO, EXPTP_SOLOUD_AUDIO);

extern "C" EXPCL_SOLOUD_AUDIO void init_libSoLoudAudio();
extern "C" EXPCL_SOLOUD_AUDIO Create_AudioManager_proc *get_audio_manager_func_soloud_audio();

extern ConfigVariableString soloud_device;
extern ConfigVariableInt soloud_buffer_delete_retries;
extern ConfigVariableDouble soloud_buffer_delete_delay;

#endif // CONFIG_SOLOUDAUDIO_H
