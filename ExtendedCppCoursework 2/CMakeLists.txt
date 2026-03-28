cmake_minimum_required(VERSION 3.22)

project(ExtendedCppCoursework VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  juce
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.2
)
FetchContent_MakeAvailable(juce)

juce_add_gui_app(ExtendedCppCoursework
  PRODUCT_NAME "ExtendedCppCoursework"
)

juce_generate_juce_header(ExtendedCppCoursework)

target_sources(ExtendedCppCoursework
  PRIVATE
    Source/Main.cpp
    Source/MainComponent.h
    Source/MainComponent.cpp
    Source/DJAudioPlayer.h
    Source/DJAudioPlayer.cpp
    Source/DeckGUI.h
    Source/DeckGUI.cpp
    Source/WaveformDisplay.h
    Source/WaveformDisplay.cpp
    Source/Interval.h
    Source/ScratchEngine.h
    Source/ScratchEngine.cpp
)

target_compile_definitions(ExtendedCppCoursework
  PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
)

target_link_libraries(ExtendedCppCoursework
  PRIVATE
    juce::juce_audio_utils
    juce::juce_dsp
    juce::juce_gui_extra
    juce::juce_recommended_config_flags
    juce::juce_recommended_warning_flags
)
