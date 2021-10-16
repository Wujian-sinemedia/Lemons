
/** @ingroup choosable
 *  @{
 */
#include "OscEngine.h"

namespace lemons::dsp::osc
{
template < typename SampleType, template < typename T > class Osctype >
Osctype< SampleType >* OscEngine< SampleType, Osctype >::operator->()
{
    return &osc;
}

template < typename SampleType, template < typename T > class Osctype >
void OscEngine< SampleType, Osctype >::setFrequency (float freqHz)
{
    frequency = freqHz;
    osc.setFrequency (frequency, (SampleType) this->getSamplerate());
}

template < typename SampleType, template < typename T > class Osctype >
void OscEngine< SampleType, Osctype >::renderBlock (const AudioBuffer< SampleType >&,
                                                    AudioBuffer< SampleType >& output,
                                                    MidiBuffer&, bool)
{
    osc.getSamples (output.getWritePointer (0), output.getNumSamples());
}

template < typename SampleType, template < typename T > class Osctype >
void OscEngine< SampleType, Osctype >::prepared (int, double samplerate)
{
    osc.resetPhase();
    osc.setFrequency (frequency, (SampleType) samplerate);
}


#define BV_DECLARE_OSC_ENGINE(Class)          \
    template class OscEngine< float, Class >; \
    template class OscEngine< double, Class >;

BV_DECLARE_OSC_ENGINE (Sine)
BV_DECLARE_OSC_ENGINE (Saw)
BV_DECLARE_OSC_ENGINE (Square)
BV_DECLARE_OSC_ENGINE (Triangle)
BV_DECLARE_OSC_ENGINE (SuperSaw)

#undef BV_DECLARE_OSC_ENGINE

}  // namespace lemons::dsp::osc

/** @}*/
