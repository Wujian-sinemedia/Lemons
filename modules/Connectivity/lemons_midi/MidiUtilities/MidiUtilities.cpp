
namespace lemons::midi
{


void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
                            MidiBuffer&       destBuffer,
                            const int         startSampleOfInput,
                            const int         startSampleOfOutput,
                            const int         numSamples)
{
    if (numSamples == 0) return;

    destBuffer.clear (startSampleOfOutput, numSamples);
    destBuffer.addEvents (readingBuffer,
                          startSampleOfInput,
                          numSamples,
                          startSampleOfOutput - startSampleOfInput);
}


float getGainMultFromMidiVelocity (float midiVelocity, float sensitivity)
{
    jassert (midiVelocity >= 0.0f && midiVelocity <= 1.0f && sensitivity >= 0.0f && sensitivity <= 1.0f);
    return (1.0f - midiVelocity) * (1.0f - sensitivity) + midiVelocity;
}

float getGainMultFromMidiVelocity (int midiVelocity, float sensitivity)
{
    jassert (midiVelocity >= 0 && midiVelocity <= 127);
    constexpr auto inv127 = 1.0f / 127.0f;
    return getGainMultFromMidiVelocity (static_cast< float > (midiVelocity) * inv127,
                                        sensitivity);
}


void VelocityHelper::setSensitivity (float newSensitivity)
{
    jassert (newSensitivity >= 0.0f && newSensitivity <= 1.0f);
    sensitivity = newSensitivity;
}

void VelocityHelper::setSensitivity (int newSensitivity)
{
    setSensitivity (static_cast< float > (newSensitivity) * 0.01f);
}

float VelocityHelper::getGainForVelocity (float midiVelocity)
{
    return getGainMultFromMidiVelocity (midiVelocity, sensitivity);
}

float VelocityHelper::getGainForVelocity (int midiVelocity)
{
    return getGainMultFromMidiVelocity (midiVelocity, sensitivity);
}


MidiFIFO::MidiFIFO (int maxNumMessages) { setSize (maxNumMessages); }

void MidiFIFO::setSize (int maxNumMessages)
{
    const auto messages = size_t (maxNumMessages);
    base.ensureSize (messages);
    copying.ensureSize (messages);
}


void MidiFIFO::clear()
{
    base.clear();
    copying.clear();
    numStoredSamples = 0;
}


int MidiFIFO::numStoredEvents() const { return base.getNumEvents(); }


void MidiFIFO::pushEvents (const MidiBuffer& source, int numSamples)
{
    base.addEvents (source, 0, numSamples, numStoredSamples);
    numStoredSamples += numSamples;
}


void MidiFIFO::popEvents (MidiBuffer& output, int numSamples)
{
    output.clear();
    output.addEvents (base, 0, numSamples, 0);

    // Move all the remaining events forward by the number of samples removed
    copying.clear();
    copying.addEvents (base, numSamples, numStoredSamples, -numSamples);

    base.swapWith (copying);
    numStoredSamples = std::max (0, numStoredSamples - numSamples);
}


float PitchPipeline::getFrequencyForMidi (int midiPitch, int midiChannel) const
{
    return getFrequencyForMidi (static_cast< float > (midiPitch), midiChannel);
}

float PitchPipeline::getFrequencyForMidi (float midiPitch, int midiChannel) const
{
    return tuning.midiToFrequency (bend.getAdjustedMidiPitch (midiPitch),
                                   midiChannel);
}

float PitchPipeline::getMidiForFrequency (int midiPitch, int midiChannel) const
{
    return getMidiForFrequency (static_cast< float > (midiPitch), midiChannel);
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


void PitchBendTracker::setRange (int newStUp, int newStDown) noexcept
{
    rangeUp   = newStUp;
    rangeDown = newStDown;
}

int PitchBendTracker::getRangeUp() const noexcept { return rangeUp; }
int PitchBendTracker::getRangeDown() const noexcept { return rangeDown; }

int PitchBendTracker::getLastRecievedPitchbend() const noexcept
{
    return lastRecievedPitchbend;
}

void PitchBendTracker::newPitchbendRecieved (const int newPitchbend)
{
    jassert (newPitchbend >= 0 && newPitchbend <= 127);
    lastRecievedPitchbend = newPitchbend;
}

}  // namespace lemons::midi
