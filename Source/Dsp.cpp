
#include "Dsp.h"

void HnDspChain::prepare(double sr, int bs, int channels)
{
    juce::dsp::ProcessSpec spec{sr,(juce::uint32)bs,(juce::uint32)channels};
    compressor.prepare(spec);
    limiter.prepare(spec);
    gain.prepare(spec);
    gain.setGainDecibels(0.0f);
    compressor.setThreshold(-18.0f);
    compressor.setRatio(3.0f);
    limiter.setThreshold(-1.0f);
}

void HnDspChain::process(juce::AudioBuffer<float>& b)
{
    juce::dsp::AudioBlock<float> block(b);
    juce::dsp::ProcessContextReplacing<float> ctx(block);
    if (enabled[2]) compressor.process(ctx);
    if (enabled[7]) limiter.process(ctx);
}

void HnDspChain::setEnabled(int index, bool e)
{
    if (index>=0 && index<8) enabled[index]=e;
}
