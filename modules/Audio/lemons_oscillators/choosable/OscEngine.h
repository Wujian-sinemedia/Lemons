
#pragma once

namespace lemons::dsp::osc
{
/** A utility class that wraps an Oscillator into a self-contained dsp::Engine.
    This will instantiate and manage the appropriate oscillator type for you. \n
    Example usage:
    @code
    using namspace lemons::dsp::osc;

    auto sineEngine = OscEngine<float, Sine>;
    @endcode
    @tparam SampleType The floating point type to be used for samples. Must be either float or double.
    @tparam Osctype A template class representing the kind of oscillator you want this engine to use; for example, osc::Sine. This should be un-specialized. This type must inherit from Oscillator.
    @see Oscillator, dsp::Engine
 */
template < typename SampleType, template < typename T > class Osctype >
class OscEngine : public dsp::Engine< SampleType >
{
public:
    using Osc = Osctype< SampleType >;

    /** Constructs a default oscillator engine. */
    OscEngine() = default;

    /** Returns a pointer to the owned Oscillator object. */
    Osc* operator->();

    /** Updates the frequency of the owned oscillator object. */
    void setFrequency (float freqHz);

private:
    void renderBlock (const AudioBuffer< SampleType >& input, AudioBuffer< SampleType >& output, MidiBuffer& midiMessages, bool isBypassed) final;
    void prepared (int blocksize, double samplerate) final;

    Osc osc;

    float frequency {440.f};
};

}  // namespace lemons::dsp::osc
