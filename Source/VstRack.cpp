
#include "VstRack.h"

VstRack::VstRack()
{
    formats.addDefaultFormats();
}

void VstRack::prepare(double sampleRate, int blockSize, int channels)
{
    sr=sampleRate; bs=blockSize; ch=channels;
    for (auto* s: instances)
        if (s->instance) s->instance->prepareToPlay(sr,bs);
}

bool VstRack::addVst3(const juce::File& file, juce::String& error)
{
    juce::PluginDescription d;
    d.fileOrIdentifier=file.getFullPathName();
    d.pluginFormatName="VST3";
    d.name=file.getFileNameWithoutExtension();

    juce::String err;
    auto inst=formats.createPluginInstance(d,sr,bs,err);
    if (!inst) { error=err; return false; }

    auto* slot=new Slot();
    slot->instance=std::move(inst);
    instances.add(slot);
    return true;
}

void VstRack::process(juce::AudioBuffer<float>& buffer)
{
    juce::MidiBuffer midi;
    for (auto* s: instances)
        if (s->instance && !s->bypass)
            s->instance->processBlock(buffer,midi);
}

void VstRack::setBypassed(int index,bool b)
{
    if (index>=0 && index<instances.size()) instances[index]->bypass=b;
}

bool VstRack::remove(int index)
{
    if (index<0 || index>=instances.size()) return false;
    instances.remove(index);
    return true;
}

juce::StringArray VstRack::names() const
{
    juce::StringArray a;
    for (auto* s: instances)
        a.add(s->instance ? s->instance->getName() : "VST");
    return a;
}
