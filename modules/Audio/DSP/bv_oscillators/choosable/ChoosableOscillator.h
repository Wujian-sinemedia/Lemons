
#pragma once

#include "OscEngine.h"

namespace bav::dsp::osc
{
enum OscType
{
    SineOsc,
    SawOsc,
    SquareOsc,
    TriangleOsc,
    SuperSawOsc
};

template < typename SampleType >
class ChoosableOscillator
{
public:
    using AudioBuffer = juce::AudioBuffer< SampleType >;

    ChoosableOscillator();

    void    setOscType (OscType newType);
    OscType getOscType() const { return type; }

    void  setFrequency (float freqHz);
    float getFrequency() const { return freq; }

    void process (AudioBuffer& output);

    void prepare (int blocksize, double samplerate);

    /* only relevant to super saw mode */
    void setDetuneAmount (int pitchSpreadCents);

private:
    OscType type {SineOsc};
    float   freq {440.f};

#define BV_ADD_OSC_ENGINE(Class) OscEngine< SampleType, Class< SampleType > >

    BV_ADD_OSC_ENGINE (Sine)
    sine;
    BV_ADD_OSC_ENGINE (Saw)
    saw;
    BV_ADD_OSC_ENGINE (Square)
    square;
    BV_ADD_OSC_ENGINE (Triangle)
    triangle;
    BV_ADD_OSC_ENGINE (SuperSaw)
    superSaw;

#undef BV_ADD_OSC_ENGINE
};

}  // namespace bav::dsp::osc
