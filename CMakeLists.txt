
cmake_minimum_required(VERSION 3.22)
project(HNStudioMusikAI VERSION 0.4.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  JUCE
  GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
  GIT_TAG 8.0.12
)
FetchContent_MakeAvailable(JUCE)

juce_add_gui_app(HNStudioMusikAI
    PRODUCT_NAME "HNStudio Musik AI"
    COMPANY_NAME "Hoai Nguyen Studio"
    BUNDLE_ID "vn.hoaimusic.hnstudio"
    VERSION "0.4.0"
)

# Generate the JUCE amalgamated header used by the source files.
# Without this, MSVC fails with C1083: cannot open include file: 'JuceHeader.h'.
juce_generate_juce_header(HNStudioMusikAI)

target_sources(HNStudioMusikAI PRIVATE
    Source/Main.cpp
    Source/MainComponent.h
    Source/MainComponent.cpp
    Source/AudioEngine.h
    Source/AudioEngine.cpp
    Source/VstRack.h
    Source/VstRack.cpp
    Source/Dsp.h
    Source/Dsp.cpp
)

target_compile_definitions(HNStudioMusikAI PRIVATE
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3HOST=1
    JUCE_PLUGINHOST_VST3=1
    JUCE_DISPLAY_SPLASH_SCREEN=0
)

target_link_libraries(HNStudioMusikAI PRIVATE
    juce::juce_audio_utils
    juce::juce_audio_processors
    juce::juce_audio_formats
    juce::juce_dsp
    juce::juce_gui_extra
)

if (WIN32)
    target_compile_definitions(HNStudioMusikAI PRIVATE
        _WIN32_WINNT=0x0A00
        WINVER=0x0A00
    )
endif()
