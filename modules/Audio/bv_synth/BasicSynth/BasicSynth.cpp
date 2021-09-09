
namespace bav::dsp
{
template < typename SampleType, template < typename NumericType > class OscType >
void BasicSynthVoice< SampleType, OscType >::renderPlease (AudioBuffer< SampleType >& output, float desiredFrequency, double currentSamplerate)
{
    osc.setFrequency (SampleType (desiredFrequency), SampleType (currentSamplerate));
    osc.getSamples (output.getWritePointer (0), output.getNumSamples());
}

template < typename SampleType, template < typename NumericType > class OscType >
void BasicSynthVoice< SampleType, OscType >::released()
{
    osc.resetPhase();
}

template < typename SampleType, template < typename NumericType > class OscType >
void BasicSynthVoice< SampleType, OscType >::noteCleared()
{
    osc.resetPhase();
}

#define bvbs_DECLARE_BASIC_SYNTH_VOICE(OscillatorType)             \
    template struct BasicSynthVoice< float, osc::OscillatorType >; \
    template struct BasicSynthVoice< double, osc::OscillatorType >

bvbs_DECLARE_BASIC_SYNTH_VOICE (Sine);
bvbs_DECLARE_BASIC_SYNTH_VOICE (Saw);
bvbs_DECLARE_BASIC_SYNTH_VOICE (Square);
bvbs_DECLARE_BASIC_SYNTH_VOICE (Triangle);
bvbs_DECLARE_BASIC_SYNTH_VOICE (SuperSaw);

#undef bvbs_DECLARE_BASIC_SYNTH_VOICE


template < typename SampleType, template < typename NumericType > class OscType >
SynthVoiceBase< SampleType >* BasicSynth< SampleType, OscType >::createVoice()
{
    return new BasicSynthVoice< SampleType, OscType > (this);
}

#define bvbs_DECLARE_BASIC_SYNTH(OscillatorType)              \
    template struct BasicSynth< float, osc::OscillatorType >; \
    template struct BasicSynth< double, osc::OscillatorType >

bvbs_DECLARE_BASIC_SYNTH (Sine);
bvbs_DECLARE_BASIC_SYNTH (Saw);
bvbs_DECLARE_BASIC_SYNTH (Square);
bvbs_DECLARE_BASIC_SYNTH (Triangle);
bvbs_DECLARE_BASIC_SYNTH (SuperSaw);

#undef bvbs_DECLARE_BASIC_SYNTH

}  // namespace bav::dsp
