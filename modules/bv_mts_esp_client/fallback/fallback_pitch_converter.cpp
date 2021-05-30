
namespace bav::midi
{
bool FallbackPitchConverter::setConcertPitchHz (float newConcertPitchHz)
{
    if (concertPitchHz == newConcertPitchHz)
        return false;

    concertPitchHz = newConcertPitchHz;
    return true;
}

float FallbackPitchConverter::midiToFrequency (int midiNote) const
{
    return midiToFrequency (static_cast< float > (midiNote));
}

float FallbackPitchConverter::midiToFrequency (float midiNote) const
{
    return static_cast< float > (concertPitchHz
                                 * std::pow (2.0f,
                                             ((midiNote - 69.0f) / 12.0f)));
}

int FallbackPitchConverter::frequencyToMidi (float frequency) const
{
    return juce::roundToInt (12.0f
                                 * log2 (frequency / concertPitchHz)
                             + 69.0f);
}

}  // namespace bav
