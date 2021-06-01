
#pragma once

namespace bav::midi
{
struct PitchPipeline
{
    PitchBendTracker bend;
    PitchConverter   tuning;


    float getFrequencyForMidi (int midiPitch, int midiChannel = -1)
    {
        return getFrequencyForMidi (static_cast< float > (midiPitch), midiChannel);
    }

    float getFrequencyForMidi (float midiPitch, int midiChannel = -1)
    {
        return tuning.midiToFrequency (bend.getAdjustedMidiPitch (midiPitch),
                                       midiChannel);
    }
    
    void reset()
    {
        bend.newPitchbendRecieved (64);
        tuning.setConcertPitchHz (440.0f);
    }
};

}  // namespace bav::midi
