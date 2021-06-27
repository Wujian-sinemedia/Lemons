
namespace bav::dsp::psola
{
template < typename SampleType >
void AnalysisGrainManager< SampleType >::analyzeInput (const SampleType* inputSamples,
                                                       int               numSamples,
                                                       int               period)
{
    grainIndices.clearQuick();
    extractor.analyzeInput (grainIndices, inputSamples, numSamples, period);
    storage.storeNewFrame (inputSamples, numSamples, grainIndices);
}

template < typename SampleType >
int AnalysisGrainManager< SampleType >::getStartOfClosestGrain (int sampleIndex) const
{
    return storage.getStartOfClosestGrain (sampleIndex);
}

template < typename SampleType >
void AnalysisGrainManager< SampleType >::prepare (int blocksize)
{
    grainIndices.ensureStorageAllocated (blocksize);
    extractor.prepare (blocksize);
    storage.prepare (blocksize);
}

template < typename SampleType >
const CircularBuffer< SampleType >& AnalysisGrainManager< SampleType >::getBuffer() const
{
    return storage.getBuffer();
}

template class AnalysisGrainManager< float >;
template class AnalysisGrainManager< double >;

}  // namespace bav::dsp::psola
