
namespace lemons::dsp::psola
{

template<typename SampleType>
inline SampleType getWindowValue (SampleType x)
{
    return (SampleType(1) + std::cos (juce::MathConstants<SampleType>::twoPi * x)) / SampleType(2);
}

template float getWindowValue (float);
template double getWindowValue (double);

/*----------------------------------------------------------------------------------------*/

template<typename SampleType>
Shifter<SampleType>::Shifter (Analyzer<SampleType>& analyzerToUse)
: analyzer(analyzerToUse)
{
}

template<typename SampleType>
void Shifter<SampleType>::setPitch (int pitchHz, double samplerate)
{
    jassert (samplerate > 0 && pitchHz > 0);
    targetPeriod = math::periodInSamples (samplerate, (float) pitchHz);
}

template<typename SampleType>
void Shifter<SampleType>::newBlockReceived (int numSamples)
{
    inptr = 0;
    outptr = 0;
    lastBlocksize = numSamples;
    placeInInputBlock = 0;
}

template<typename SampleType>
void Shifter<SampleType>::getSamples (AudioBuffer<SampleType>& output)
{
    getSamples (output.getWritePointer(0), output.getNumSamples());
}

template<typename SampleType>
void Shifter<SampleType>::getSamples (SampleType* output, int numSamples)
{
    jassert (targetPeriod > 0.f);
    
    const auto periodRatio = targetPeriod / analyzer.currentPeriod;
    
    while (placeInInputBlock + inptr < lastBlocksize && outptr < numSamples)
    {
        if ((outptr / numSamples) < (inptr / lastBlocksize))
        {
            outptr += (juce::roundToInt (analyzer.currentPeriod * periodRatio));
            
            const auto intPeriod = juce::roundToInt (analyzer.currentPeriod);
            
            const auto* input = analyzer.storage.getReadPointer(0);
            
            for (int n = -intPeriod; n <= intPeriod; ++n)
            {
                output[n + outptr] += (input[n + inptr] * getWindowValue (SampleType(n) / analyzer.currentPeriod));
            }
        }
        else
        {
            ++inptr;
        }
    }
    
    placeInInputBlock += numSamples;
}

template class Shifter<float>;
template class Shifter<double>;

}
