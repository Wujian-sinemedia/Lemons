
namespace lemons::dsp::buffers
{
template < typename SampleType >
void copy (const AudioBuffer< SampleType >& source, AudioBuffer< SampleType >& dest)
{
    dest.clear();

    const auto numSamples = source.getNumSamples();
    jassert (dest.getNumSamples() >= numSamples);

    const auto numChannels = std::min (source.getNumChannels(), dest.getNumChannels());

    for (int chan = 0; chan < numChannels; ++chan)
        vecops::copy (source.getReadPointer (chan), dest.getWritePointer (chan), numSamples);
}

template void copy (const AudioBuffer< float >&, AudioBuffer< float >&);
template void copy (const AudioBuffer< double >&, AudioBuffer< double >&);

template < typename Type1, typename Type2 >
void convert (const AudioBuffer< Type1 >& source, AudioBuffer< Type2 >& dest)
{
    dest.clear();

    const auto numSamples = source.getNumSamples();
    jassert (dest.getNumSamples() >= numSamples);

    const auto numChannels = std::min (source.getNumChannels(), dest.getNumChannels());

    for (int chan = 0; chan < numChannels; ++chan)
        vecops::convert (dest.getWritePointer (chan), source.getReadPointer (chan), numSamples);
}
template void convert (const AudioBuffer< float >&, AudioBuffer< double >&);
template void convert (const AudioBuffer< double >&, AudioBuffer< float >&);


template < typename SampleType >
AudioBuffer< SampleType > getAliasBuffer (AudioBuffer< SampleType >& bufferToAlias,
                                          int                        startSample,
                                          int                        numSamples,
                                          int                        numChannels,
                                          int                        channelOffset)
{
    return {bufferToAlias.getArrayOfWritePointers() + channelOffset, numChannels, startSample, numSamples};
}
template AudioBuffer< float >  getAliasBuffer (AudioBuffer< float >&, int, int, int, int);
template AudioBuffer< double > getAliasBuffer (AudioBuffer< double >&, int, int, int, int);

}  // namespace lemons::dsp::buffers
