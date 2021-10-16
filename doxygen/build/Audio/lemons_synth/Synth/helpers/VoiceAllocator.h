
/** @ingroup Synth
 *  @{
 */

/** @ingroup helpers
 *  @{
 */
#pragma once

namespace lemons::dsp::synth
{
template < typename SampleType >
class VoiceAllocator
{
public:
    using Voice = SynthVoiceBase< SampleType >;

    VoiceAllocator (SynthBase< SampleType >& s);

    void prepare (int maxNumVoices);

    Voice* findFreeVoice (bool stealIfNoneAvailable);

    Voice* findVoiceToSteal();

private:
    SynthBase< SampleType >& synth;

    Array< Voice* > usableVoices;  // this array is used to sort the voices when a 'steal' is requested
};

}  // namespace lemons::dsp::synth

/** @}*/

/** @}*/
