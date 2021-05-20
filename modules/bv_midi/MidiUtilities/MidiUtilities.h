
namespace bav::midi
{
// returns true if the given midinote is a black key
template < typename noteType >
inline bool isMidiNoteBlackKey (noteType midipitch)
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


// returns true if the two pitches are the same pitch class, regardless of octave
template < typename noteType1, typename noteType2 >
inline bool areNotesSamePitchClass (const noteType1 pitch1, const noteType2 pitch2)
{
    jassert (pitch1 >= noteType1 (0) && pitch2 >= noteType2 (0)
             && pitch1 <= noteType1 (127) && pitch2 <= noteType2 (127));
    return (pitch1 % 12 == pitch2 % 12);
}


// copies a range of events from one juce::MidiBuffer to another, optionally providing a sample offset
inline void copyRangeOfMidiBuffer (const juce::MidiBuffer& readingBuffer,
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


}  // namespace bav::midi
