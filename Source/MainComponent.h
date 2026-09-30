
#pragma once
#include <JuceHeader.h>
#include "AudioEngine.h"

class MainComponent : public juce::Component, private juce::Timer
{
public:
    MainComponent();
    ~MainComponent() override;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void timerCallback() override;
    void addVst();
    void audioSetup();
    void showGuide();
    void setLive();
    void toggleStereoMix();

    AudioEngine engine;
    juce::Label title;
    juce::ToggleButton live{"LIVE OFF"};
    juce::ToggleButton stereo{"Stereo Mix"};
    juce::TextButton addVstBtn{"+ ADD VST"};
    juce::TextButton audioBtn{"AUDIO I/O"};
    juce::TextButton guideBtn{"HƯỚNG DẪN"};
    juce::Slider micVol, musicVol, masterVol;
    juce::Label meters;
    juce::ListBox vstList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
