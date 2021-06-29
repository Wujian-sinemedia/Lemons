
namespace bav::dsp::buffers
{

template<typename SampleType>
void copy (const juce::AudioBuffer<SampleType>& source, juce::AudioBuffer<SampleType>& dest)
{
    dest.clear();
    
    const auto numSamples = source.getNumSamples();
    jassert (dest.getNumSamples() >= numSamples);
    
    const auto numChannels = std::min (source.getNumChannels(), dest.getNumChannels());
    
    for (int chan = 0; chan < numChannels; ++chan)
        vecops::copy (source.getReadPointer (chan), dest.getWritePointer (chan), numSamples);
}

template void copy (const juce::AudioBuffer<float>&, juce::AudioBuffer<float>&);
template void copy (const juce::AudioBuffer<double>&, juce::AudioBuffer<double>&);

}
