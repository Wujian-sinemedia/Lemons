#pragma once

#include "OscEngine.h"
#include <lemons_serializing/lemons_serializing.h>

namespace lemons::dsp::osc
{
/** An enum class that represents the kinds of oscillator that ChoosableOscillator encapsulates.
    @see ChoosableOscillator
 */
enum class OscType
{
    Sine,
    Saw,
    Square,
    Triangle,
    SuperSaw
};


/** A class that encapsulates together all the kinds of oscillators, and allows you to seamlessly switch between them during playback.
    @see OscType, OscEngine, Oscillator
 */
template < typename SampleType >
class ChoosableOscillator : public SerializableData
{
public:
    /** Constructs a default choosable oscillator. */
    ChoosableOscillator();

    /** Sets the type of oscillator that will be used to generate the output.
        @see OscType
     */
    void setOscType (OscType newType);

    /** Returns the type of oscillator that is currently chosen. */
    OscType getOscType() const { return type; }

    /** Sets the oscillator's frequency. */
    void setFrequency (float freqHz);

    /** Returns the frequency of the oscillator. */
    float getFrequency() const { return freq; }

    /** Processes a stream of audio. */
    void process (AudioBuffer< SampleType >& output);

    /** Prepares the choosable oscillator for playback. */
    void prepare (int blocksize, double samplerate);

    /** Sets the detune amount for the super saw oscillator.
        This has no effect on the other oscillator types.
        @see SuperSaw::setDetuneAmount()
     */
    void setDetuneAmount (int pitchSpreadCents);


private:
    /** If you're implementing a subclass of ChoosableOscillator, this will be called after the main call to prepare() is made. */
    virtual void prepared (int /*blocksize*/) { }

    void serialize (TreeReflector& ref) final;

    OscType type {OscType::Sine};
    float   freq {440.f};

    OscEngine< SampleType, Sine >     sine;
    OscEngine< SampleType, Saw >      saw;
    OscEngine< SampleType, Square >   square;
    OscEngine< SampleType, Triangle > triangle;
    OscEngine< SampleType, SuperSaw > superSaw;
};

}  // namespace lemons::dsp::osc
