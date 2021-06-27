
namespace bav::dsp::psola
{
template < typename SampleType >
void AnalysisGrainStorage< SampleType >::prepare (int blocksize)
{
    buffer.resize (blocksize, 4);
    grainIndices.ensureStorageAllocated (blocksize);
    distances.ensureStorageAllocated (blocksize);
}

template < typename SampleType >
void AnalysisGrainStorage< SampleType >::storeNewFrame (const SampleType*         inputSamples,
                                                        int                       numSamples,
                                                        const juce::Array< int >& grainOnsets)
{
    jassert (! grainOnsets.isEmpty());

    buffer.storeSamples (inputSamples, numSamples);

    const auto lastFrameStart = buffer.getLastFrameStartIndex();

    grainIndices.clearQuick();

    for (auto index : grainOnsets)
        grainIndices.add (buffer.clipValueToCapacity (index + lastFrameStart));
}

template < typename SampleType >
int AnalysisGrainStorage< SampleType >::getStartOfClosestGrain (int sampleIndex) const
{
    const auto index = buffer.clipValueToCapacity (sampleIndex + buffer.getLastFrameStartIndex());

    distances.clearQuick();

    for (auto grain : grainIndices)
        distances.add (std::abs (grain - index));

    return grainIndices[intops::findIndexOfMinElement (distances.getRawDataPointer(), distances.size())];
}

template < typename SampleType >
const CircularBuffer< SampleType >& AnalysisGrainStorage< SampleType >::getBuffer() const
{
    return buffer;
}

template class AnalysisGrainStorage< float >;
template class AnalysisGrainStorage< double >;

}  // namespace bav::dsp::psola
