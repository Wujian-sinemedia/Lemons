
#pragma once

#include "OscEngine.h"

namespace bav::dsp::osc
{
template < typename SampleType >
class ChoosableOscillator
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    enum OscType
    {
        SineOsc,
        SawOsc,
        SquareOsc,
        TriangleOsc
    };

    void    setOscType (OscType newType);
    OscType getOscType() const { return type; }

    void process (AudioBuffer& output);

    void prepare (int blocksize, double samplerate);

private:
    OscEngine< SampleType, Sine< SampleType > >     sine;
    OscEngine< SampleType, Saw< SampleType > >      saw;
    OscEngine< SampleType, Square< SampleType > >   square;
    OscEngine< SampleType, Triangle< SampleType > > triangle;

    OscType type;
};

}  // namespace bav::dsp::osc
