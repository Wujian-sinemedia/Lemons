#include <lemons_core/lemons_core.h>

namespace lemons::midi
{

float PitchPipeline::getFrequencyForMidi (int midiPitch, int midiChannel) const
{
    return getFrequencyForMidi (static_cast<float> (midiPitch), midiChannel);
}

float PitchPipeline::getFrequencyForMidi (float midiPitch, int midiChannel) const
{
    return tuning.midiToFrequency (bend.getAdjustedMidiPitch (midiPitch),
                                   midiChannel);
}

float PitchPipeline::getMidiForFrequency (int midiPitch, int midiChannel) const
{
    return getMidiForFrequency (static_cast<float> (midiPitch), midiChannel);
}

float PitchPipeline::getMidiForFrequency (float midiPitch, int midiChannel) const
{
    return bend.getAdjustedMidiPitch (tuning.frequencyToMidi (math::midiToFreq (midiPitch),
                                                              midiChannel));
}

void PitchPipeline::reset()
{
    bend.newPitchbendRecieved (64);
}

}
