
#include "AudioEngine.h"

AudioEngine::AudioEngine()
{
    formats_.registerBasicFormats();
    deviceManager_.initialise(2, 2, nullptr, true);
    deviceManager_.addAudioCallback(this);
}

AudioEngine::~AudioEngine()
{
    stop();
    deviceManager_.removeAudioCallback(this);
}

bool AudioEngine::start()
{
    return deviceManager_.getCurrentAudioDevice() != nullptr;
}

void AudioEngine::stop() {}

void AudioEngine::audioDeviceAboutToStart(juce::AudioIODevice* device)
{
    const auto sr = device->getCurrentSampleRate();
    const auto bs = device->getCurrentBufferSizeSamples();
    micChain_.prepare(sr, bs, 2);
    musicChain_.prepare(sr, bs, 2);
    masterChain_.prepare(sr, bs, 2);
    vstRack_.prepare(sr, bs, 2);
}

void AudioEngine::audioDeviceStopped() {}

void AudioEngine::audioDeviceIOCallbackWithContext(
    const float* const* inputChannelData, int numInputChannels,
    float* const* outputChannelData, int numOutputChannels,
    int numSamples, const juce::AudioIODeviceCallbackContext&)
{
    juce::AudioBuffer<float> mic(2, numSamples);
    juce::AudioBuffer<float> music(2, numSamples);
    juce::AudioBuffer<float> master(2, numSamples);
    mic.clear(); music.clear(); master.clear();

    // MIC PATH: isolated. Never mixed into the music path.
    for (int ch=0; ch<2; ++ch)
        if (ch < numInputChannels && inputChannelData[ch])
            mic.copyFrom(ch, 0, inputChannelData[ch], numSamples, micVolume);

    // MUSIC PATH is deliberately independent. In a complete build, this is fed by
    // a file player or a Windows WASAPI loopback capture source. It never uses mic data.
    // Stereo Mix only controls whether a system-loopback source is enabled.
    // It does NOT gate the internal SFX bus.
    juce::ignoreUnused(stereoMix);

    micChain_.process(mic);
    vstRack_.process(mic);

    // Keep the two buses independent. The current scaffold mixes the processed mic
    // into the master only at the final stage; the music bus remains a separate bus.
    // This guarantees Stereo Mix cannot accidentally route into the mic chain.
    for (int ch = 0; ch < master.getNumChannels(); ++ch)
        master.copyFrom(ch, 0, mic, ch, 0, numSamples);
master.applyGain(ch, 0, numSamples, masterVolume);

    masterChain_.process(master);

    for (int ch = 0; ch < numOutputChannels; ++ch)
        if (outputChannelData[ch])
            juce::FloatVectorOperations::copy(outputChannelData[ch],
                master.getReadPointer(juce::jmin(ch, master.getNumChannels() - 1)), numSamples);

    const auto rms = [](const juce::AudioBuffer<float>& b) {
        return b.getRMSLevel(0,0,b.getNumSamples());
    };
    micRms.store(rms(mic));
    musicRms.store(rms(music));
    masterRms.store(rms(master));
}

void AudioEngine::setEffectEnabled(int index, bool enabled)
{
    micChain_.setEnabled(index, enabled);
}

void AudioEngine::triggerLaugh()
{
    // Dedicated SFX bus hook. It is intentionally independent from Stereo Mix.
}
