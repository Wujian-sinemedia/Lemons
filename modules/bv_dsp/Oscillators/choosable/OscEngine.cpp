
namespace bav::dsp::osc
{
template < typename SampleType, class OscType >
OscEngine< SampleType, OscType >::OscEngine()
{
    static_assert (std::is_same< SampleType, typename OscType::NumericType >::value,
                   "The oscillator engine's SampleType must match its oscillator's SampleType!");
}

template < typename SampleType, class OscType >
void OscEngine< SampleType, OscType >::setFrequency (float freqHz)
{
    frequency = freqHz;
    osc.setFrequency (frequency, (SampleType) Engine< SampleType >::getSamplerate());
}

template < typename SampleType, class OscType >
void OscEngine< SampleType, OscType >::renderBlock (const AudioBuffer&,
                                                    AudioBuffer& output,
                                                    MidiBuffer&, bool)
{
    osc.getSamples (output.getWritePointer (0), output.getNumSamples());
}

template < typename SampleType, class OscType >
void OscEngine< SampleType, OscType >::prepared (int, double samplerate)
{
    osc.resetPhase();
    osc.setFrequency (frequency, (SampleType) samplerate);
}

template class OscEngine< float, Sine< float > >;
template class OscEngine< double, Sine< double > >;
template class OscEngine< float, Saw< float > >;
template class OscEngine< double, Saw< double > >;
template class OscEngine< float, Square< float > >;
template class OscEngine< double, Square< double > >;
template class OscEngine< float, Triangle< float > >;
template class OscEngine< double, Triangle< double > >;

}  // namespace bav::dsp::osc
