
namespace bav::midi
{
template < typename noteType >
bool isMidiNoteBlackKey (noteType midipitch)
{
    jassert (midipitch >= noteType (0) && midipitch <= noteType (127));

    switch (midipitch % 12)
    {
        case 1 : return true;
        case 3 : return true;
        case 6 : return true;
        case 8 : return true;
        case 10 : return true;
        default : return false;
    }
}

template < typename noteType1, typename noteType2 >
bool areNotesSamePitchClass (const noteType1 pitch1, const noteType2 pitch2)
{
    jassert (pitch1 >= noteType1 (0) && pitch2 >= noteType2 (0)
             && pitch1 <= noteType1 (127) && pitch2 <= noteType2 (127));
    return (pitch1 % 12 == pitch2 % 12);
}


void copyRangeOfMidiBuffer (const juce::MidiBuffer& readingBuffer,
                            juce::MidiBuffer&       destBuffer,
                            const int               startSampleOfInput,
                            const int               startSampleOfOutput,
                            const int               numSamples)
{
    destBuffer.clear (startSampleOfOutput, numSamples);
    destBuffer.addEvents (readingBuffer,
                          startSampleOfInput,
                          numSamples,
                          startSampleOfOutput - startSampleOfInput);
}


bool isValidMidiPitch (int note)
{
    return note >= 0 && note <= 127;
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
    return getGainMultFromMidiVelocity (static_cast<float> (midiVelocity) * inv127,
                                        sensitivity);
}


void VelocityHelper::setSensitivity (float newSensitivity)
{
    jassert (newSensitivity >= 0.0f && newSensitivity <= 1.0f);
    sensitivity = newSensitivity;
}

void VelocityHelper::setSensitivity (int newSensitivity)
{
    setSensitivity (static_cast<float> (newSensitivity) * 0.01f);
}

float VelocityHelper::getGainForVelocity (float midiVelocity)
{
    return getGainMultFromMidiVelocity (midiVelocity, sensitivity);
}

float VelocityHelper::getGainForVelocity (int midiVelocity)
{
    return getGainMultFromMidiVelocity (midiVelocity, sensitivity);
}

}  // namespace bav::midi
