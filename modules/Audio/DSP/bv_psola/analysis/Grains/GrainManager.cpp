
namespace bav::dsp::psola
{

template < typename SampleType >
void AnalysisGrainManager< SampleType >::analyzeInput (const SampleType* inputSamples,
                                                       int               numSamples,
                                                       int               period)
{
    extractor.analyzeInput (inputSamples, numSamples, period);
}

template < typename SampleType >
void AnalysisGrainManager< SampleType >::prepare (int blocksize)
{
    extractor.prepare (blocksize);
}

template class AnalysisGrainManager< float >;
template class AnalysisGrainManager< double >;

}  // namespace bav::dsp::psola
