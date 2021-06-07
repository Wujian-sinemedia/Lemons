#pragma once

namespace bav::dsp::FX
{

template<typename SampleType>
class DryWetMixer
{
public:
    using AudioBuffer = juce::AudioBuffer<SampleType>;
    
    DryWetMixer()
    {
        mixer.setMixingRule (juce::dsp::DryWetMixingRule::balanced);
        setWetLatency (0);
    }
    
    void setWetMix (int wetPercent)
    {
        mixer.setWetMixProportion (SampleType(wetPercent) * SampleType(0.01));
    }
    
    void setWetLatency (int latencySamples)
    {
        mixer.setWetLatency (SampleType (latencySamples));
    }
    
    void prepare (int numChannels, int blocksize, double samplerate)
    {
        spec.numChannels = juce::uint32 (numChannels);
        spec.maximumBlockSize = juce::uint32 (blocksize);
        spec.sampleRate = samplerate;
        
        mixer.prepare (spec);
    }
    
    void reset() { mixer.reset(); }
    
    void pushDrySamples (AudioBuffer& buffer)
    {
        mixer.pushDrySamples (juce::dsp::AudioBlock< SampleType > (buffer));
    }
    
    void mixWetSamples (AudioBuffer& buffer)
    {
        mixer.mixWetSamples (juce::dsp::AudioBlock< SampleType > (buffer));
    }
    
private:
    juce::dsp::DryWetMixer< SampleType > mixer {512};
    juce::dsp::ProcessSpec               spec;
};

template class DryWetMixer<float>;
template class DryWetMixer<double>;

}
