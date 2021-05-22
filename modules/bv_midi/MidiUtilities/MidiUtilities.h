
#pragma once

namespace bav::midi
{
// returns true if the given midinote is a black key
template < typename noteType >
bool isMidiNoteBlackKey (noteType midipitch);


// returns true if the two pitches are the same pitch class, regardless of octave
template < typename noteType1, typename noteType2 >
bool areNotesSamePitchClass (const noteType1 pitch1, const noteType2 pitch2);


// copies a range of events from one juce::MidiBuffer to another, optionally providing a sample offset
void copyRangeOfMidiBuffer (const juce::MidiBuffer& readingBuffer,
                            juce::MidiBuffer&       destBuffer,
                            const int               startSampleOfInput,
                            const int               startSampleOfOutput,
                            const int               numSamples);

// returns true if the passed int is between 0 and 127
bool isValidMidiPitch (int note);


}  // namespace bav::midi
