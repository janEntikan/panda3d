set(P3MINIAUDIO_HEADERS
  config_miniAudio.h
  miniaudio.h miniaudio_experimental.h
  miniaudio_reverb.h
  miniAudioManager.h
  miniAudioSound.I miniAudioSound.h
)

set (P3MINIAUDIO_SOURCES
  config_miniAudio.cxx miniAudioManager.cxx miniAudioSound.cxx
)
composite_sources(p3miniaudio P3MINIAUDIO_SOURCES)
add_library(p3miniaudio ${MODULE_TYPE} ${P3MINIAUDIO_HEADERS} ${P3MINIAUDIO_SOURCES})
set_target_properties(p3miniaudio PROPERTIES DEFINE_SYMBOL BUILDING_MINIAUDIO)
target_link_libraries(p3miniaudio p3audio panda)

install(TARGETS p3miniaudio
  EXPORT Miniaudio COMPONENT Miniaudio
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/panda3d
  ARCHIVE COMPONENT MiniaudioDevel)
install(FILES ${P3MINIAUDIO_HEADERS} COMPONENT MiniaudioDevel DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/panda3d)
