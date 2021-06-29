
namespace bav::dsp
{
template < typename SampleType >
void SineSynthVoice< SampleType >::renderPlease (juce::AudioBuffer< SampleType >& output, float desiredFrequency, double currentSamplerate)
{
    sine.setFrequency (SampleType (desiredFrequency), SampleType (currentSamplerate));
    sine.getSamples (output.getWritePointer (0), output.getNumSamples());
}

template < typename SampleType >
void SineSynthVoice< SampleType >::released()
{
    sine.resetPhase();
}

template < typename SampleType >
void SineSynthVoice< SampleType >::noteCleared()
{
    sine.resetPhase();
}

template class SineSynthVoice< float >;
template class SineSynthVoice< double >;

template struct SineSynth<float>;
template struct SineSynth<double>;

}  // namespace bav::dsp
