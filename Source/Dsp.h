
#pragma once
#include <JuceHeader.h>

class HnDspChain
{
public:
    void prepare(double sampleRate, int blockSize, int channels);
    void process(juce::AudioBuffer<float>& buffer);
    void setEnabled(int index, bool enabled);
private:
    bool enabled[8]{true,true,true,true,true,true,true,true};
    juce::dsp::IIR::Filter<float> lowPass;
    juce::dsp::Compressor<float> compressor;
    juce::dsp::Limiter<float> limiter;
    juce::dsp::Gain<float> gain;
};
