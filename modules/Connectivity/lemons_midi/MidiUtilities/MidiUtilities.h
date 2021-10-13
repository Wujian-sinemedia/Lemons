
#pragma once

namespace lemons::midi
{

// copies a range of events from one juce::MidiBuffer to another, optionally providing a sample offset
void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
                            MidiBuffer&       destBuffer,
                            const int         startSampleOfInput,
                            const int         startSampleOfOutput,
                            const int         numSamples);


struct VelocityHelper
{
    void setSensitivity (float newSensitivity);
    void setSensitivity (int newSensitivity);

    float getGainForVelocity (float midiVelocity);
    float getGainForVelocity (int midiVelocity);

    float getSensitivity() const { return sensitivity; }

private:
    float sensitivity {1.0f};
};


}  // namespace lemons::midi
