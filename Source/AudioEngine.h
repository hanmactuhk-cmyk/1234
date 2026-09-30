
#pragma once
#include <JuceHeader.h>
#include "Dsp.h"
#include "VstRack.h"

class AudioEngine : public juce::AudioIODeviceCallback
{
public:
    AudioEngine();
    ~AudioEngine() override;

    bool start();
    void stop();

    void audioDeviceIOCallbackWithContext(const float* const* inputChannelData, int numInputChannels,
                                          float* const* outputChannelData, int numOutputChannels,
                                          int numSamples, const juce::AudioIODeviceCallbackContext& context) override;
    void audioDeviceAboutToStart(juce::AudioIODevice*) override;
    void audioDeviceStopped() override;

    juce::AudioDeviceManager& deviceManager() { return deviceManager_; }
    VstRack& vstRack() { return vstRack_; }

    void setMicVolume(float v) { micVolume = v; }
    void setMusicVolume(float v) { musicVolume = v; }
    void setMasterVolume(float v) { masterVolume = v; }

    void setStereoMix(bool b) { stereoMix = b; }
    bool getStereoMix() const { return stereoMix; }

    void setEffectEnabled(int index, bool enabled);
    void triggerLaugh();
    float getMicRms() const { return micRms.load(); }
    float getMusicRms() const { return musicRms.load(); }
    float getMasterRms() const { return masterRms.load(); }

private:
    juce::AudioDeviceManager deviceManager_;
    juce::AudioFormatManager formats_;
    juce::AudioSourcePlayer musicPlayer_;
    std::unique_ptr<juce::AudioTransportSource> transport_;
    std::unique_ptr<juce::AudioFormatReaderSource> musicSource_;

    VstRack vstRack_;
    HnDspChain micChain_;
    HnDspChain musicChain_;
    HnDspChain masterChain_;

    std::atomic<float> micRms{0}, musicRms{0}, masterRms{0};
    float micVolume=1.0f, musicVolume=1.0f, masterVolume=0.9f;
    bool stereoMix=false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioEngine)
};
