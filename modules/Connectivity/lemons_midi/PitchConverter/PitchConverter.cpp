
namespace lemons::midi
{
void PitchConverter::Deleter::operator() (MTSClient* c)
{
    MTS_DeregisterClient (c);
}

float PitchConverter::midiToFrequency (int midiNote, int midiChannel) const
{
    return static_cast< float > (MTS_NoteToFrequency (client.get(), char (midiNote), char (midiChannel)));
}

float PitchConverter::midiToFrequency (float midiNote, int midiChannel) const
{
    return static_cast< float > (MTS_NoteToFrequency (client.get(), char (midiNote), char (midiChannel)));
}

int PitchConverter::frequencyToMidi (float frequency, int midiChannel) const
{
    return juce::roundToInt (MTS_FrequencyToNote (client.get(), static_cast< double > (frequency), char (midiChannel)));
}

bool PitchConverter::shouldFilterNote (int midiPitch, int midiChannel) const
{
    return MTS_ShouldFilterNote (client.get(), char (midiPitch), char (midiChannel));
}

bool PitchConverter::isConnectedToMtsEsp() const
{
    return MTS_HasMaster (client.get());
}

String PitchConverter::getScaleName() const
{
    return {MTS_GetScaleName (client.get())};
}

}  // namespace lemons::midi
