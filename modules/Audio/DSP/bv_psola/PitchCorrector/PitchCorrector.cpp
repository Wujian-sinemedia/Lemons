
namespace bav::dsp::psola
{

template<typename SampleType>
PitchCorrectorBase<SampleType>::PitchCorrectorBase (Analyzer& analyzerToUse, const midi::PitchPipeline* pitchConverterToUse)
: analyzer(analyzerToUse), pitch(pitchConverterToUse)
{
    
}

template<typename SampleType>
void PitchCorrectorBase<SampleType>::processNextFrame (AudioBuffer& output)
{
    shifter.setPitch (getTargetFrequency(), sampleRate);
    shifter.getSamples (output);
}

template<typename SampleType>
void PitchCorrectorBase<SampleType>::prepare (double samplerate, int)
{
    sampleRate = samplerate;
}

template<typename SampleType>
float PitchCorrectorBase<SampleType>::getTargetFrequency()
{
    if (pitch != nullptr)
    {
        const auto inputPitch = pitch->getMidiForFrequency (analyzer.getFrequency());
        return pitch->getFrequencyForMidi (juce::roundToInt (inputPitch));
    }
    
    const auto inputPitch = math::freqToMidi (analyzer.getFrequency());
    return math::midiToFreq (juce::roundToInt (inputPitch));
}

template class PitchCorrectorBase<float>;
template class PitchCorrectorBase<double>;


template<typename SampleType>
PitchCorrector<SampleType>::PitchCorrector (const midi::PitchPipeline* pitchConverterToUse)
: PitchCorrectorBase<SampleType>(analyzer, pitchConverterToUse)
{
    
}

template class PitchCorrector<float>;
template class PitchCorrector<double>;

}
