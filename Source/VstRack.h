
#pragma once
#include <JuceHeader.h>

class VstRack
{
public:
    VstRack();
    void prepare(double sampleRate, int blockSize, int channels);
    void process(juce::AudioBuffer<float>& buffer);

    bool addVst3(const juce::File& file, juce::String& error);
    bool remove(int index);
    void setBypassed(int index, bool bypassed);
    juce::StringArray names() const;
    int size() const { return instances.size(); }

    juce::KnownPluginList& knownPlugins() { return known; }

private:
    juce::AudioPluginFormatManager formats;
    juce::KnownPluginList known;
    struct Slot { std::unique_ptr<juce::AudioPluginInstance> instance; bool bypass=false; };
    juce::OwnedArray<Slot> instances;
    double sr=48000.0; int bs=256; int ch=2;
};
