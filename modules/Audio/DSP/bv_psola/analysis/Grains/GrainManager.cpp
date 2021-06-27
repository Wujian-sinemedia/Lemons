
namespace bav::dsp::psola
{
template < typename SampleType >
void AnalysisGrainManager< SampleType >::analyzeInput (const SampleType* inputSamples,
                                                       int               numSamples,
                                                       int               period)
{
    extractor.analyzeInput (inputSamples, numSamples, period);
    storage.storeNewFrame (inputSamples, numSamples, extractor.getIndices());
}

template < typename SampleType >
int AnalysisGrainManager< SampleType >::getStartOfClosestGrain (int sampleIndex) const
{
    return storage.getStartOfClosestGrain (sampleIndex);
}

template < typename SampleType >
void AnalysisGrainManager< SampleType >::prepare (int blocksize)
{
    extractor.prepare (blocksize);
    storage.prepare (blocksize);
}

template < typename SampleType >
const AnalysisGrainStorage< SampleType >& AnalysisGrainManager< SampleType >::getStorage() const
{
    return storage;
}

template class AnalysisGrainManager< float >;
template class AnalysisGrainManager< double >;

}  // namespace bav::dsp::psola
