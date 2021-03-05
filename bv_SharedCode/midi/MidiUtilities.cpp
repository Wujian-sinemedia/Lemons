
#include "bv_SharedCode/bv_SharedCode.h"


namespace bav
{
    
namespace midi
{
    
    void copyRangeOfMidiBuffer (const juce::MidiBuffer& readingBuffer, juce::MidiBuffer& destBuffer,
                                const int startSampleOfInput,
                                const int startSampleOfOutput,
                                const int numSamples)
    {
        destBuffer.clear (startSampleOfOutput, numSamples);
        
        auto midiIterator = readingBuffer.findNextSamplePosition(startSampleOfInput);
        
        if (midiIterator == readingBuffer.cend())
            return;
        
        const auto midiEnd = readingBuffer.findNextSamplePosition(startSampleOfInput + numSamples);
        
        if (midiIterator == midiEnd)
            return;
        
        const int sampleOffset = startSampleOfOutput - startSampleOfInput;
        
        std::for_each (midiIterator, midiEnd,
                       [&] (const juce::MidiMessageMetadata& meta)
                       {
                           destBuffer.addEvent (meta.getMessage(),
                                                std::max (0, meta.samplePosition + sampleOffset));
                       } );
    }
    
    /*
     */
    
    PitchConverter::PitchConverter(const int initialConcertPitch, const int initialRootNote, const int initialNotesPerOctave):
    concertPitchHz(initialConcertPitch), rootNote(initialRootNote), notesPerOctave(initialNotesPerOctave)
    { }
    
    int PitchConverter::mtof (int midiNote) const
    {
        jassert (midiNote >= 0 && midiNote <= 127);
        return juce::roundToInt(concertPitchHz.load() * std::pow(2, ((midiNote - rootNote.load()) / notesPerOctave.load())));
    }
    
    float PitchConverter::mtof (float midiNote) const
    {
        jassert (midiNote >= 0.0f && midiNote <= 127.0f);
        return float(concertPitchHz.load() * std::pow(2.0f, ((midiNote - rootNote.load()) / notesPerOctave.load())));
    }
    
    int PitchConverter::ftom (const int inputFreq) const
    {
        jassert (inputFreq > 0);
        return juce::roundToInt(notesPerOctave.load() * log2(inputFreq / concertPitchHz.load()) + rootNote.load());
    }
    
    float PitchConverter::ftom (const float inputFreq) const
    {
        jassert (inputFreq > 0.0f);
        return float(notesPerOctave.load() * log2(inputFreq / concertPitchHz.load()) + rootNote.load());
    }
    
    void PitchConverter::setConcertPitchHz (const int newConcertPitch) noexcept
    {
        jassert (newConcertPitch > 0);
        concertPitchHz.store (newConcertPitch);
    }
    
    void PitchConverter::setNotesPerOctave (const int newNPO) noexcept
    {
        jassert (newNPO > 0);
        notesPerOctave.store (newNPO);
    }
    
    void PitchConverter::setRootNote (const int newRoot) noexcept
    {
        jassert (newRoot > 0);
        rootNote.store (newRoot);
    }
    
    
    /*
     */
    
    
    PitchBendHelper::PitchBendHelper(const int initialStUp, const int initialStDwn):
    rangeUp(initialStUp), rangeDown(initialStDwn), lastRecievedPitchbend(64)
    { }
    
    void PitchBendHelper::setRange (const int newStUp, const int newStDown) noexcept
    {
        rangeUp.store (newStUp);
        rangeDown.store (newStDown);
    }
    
    float PitchBendHelper::newNoteRecieved (const int newMidiPitch) const
    {
        return getMidifloat (juce::jlimit (0, 127, newMidiPitch),
                             lastRecievedPitchbend.load());
    }
    
    void PitchBendHelper::newPitchbendRecieved (const int newPitchbend) noexcept
    {
        lastRecievedPitchbend.store (juce::jlimit (0, 127, newPitchbend));
    }
    
    float PitchBendHelper::getMidifloat (const int midiPitch, const int pitchbend) const
    {
        jassert (juce::isPositiveAndBelow(midiPitch, 128) && juce::isPositiveAndBelow(pitchbend, 128));
        
        if (pitchbend == 64)
            return float(midiPitch);
        
        if (pitchbend > 64)
            return float(((rangeUp.load() * (pitchbend - 65)) / 62) + midiPitch);
        
        const int currentdownrange = rangeDown.load();
        return float((((1 - currentdownrange) * pitchbend) / 63) + midiPitch - currentdownrange);
    }
    
    
    /*
     */
    
    
    VelocityHelper::VelocityHelper(const int initialSensitivity): sensitivity(initialSensitivity/100.0f)
    { }
    
    void VelocityHelper::setSensitivity (int newSensitivity) noexcept
    {
        newSensitivity = juce::jlimit (0, 100, newSensitivity);
        sensitivity.store (newSensitivity / 100.0f);
    }
    
    void VelocityHelper::setFloatSensitivity (const float newSensitivity) noexcept
    {
        sensitivity.store (juce::jlimit (0.0f, 1.0f, newSensitivity));
    }
    
    float VelocityHelper::intVelocity (int midiVelocity) const
    {
        midiVelocity = juce::jlimit (0, 127, midiVelocity);
        return getGainMult (midiVelocity / 127.0f, sensitivity.load());
    }
    
    float VelocityHelper::floatVelocity (float floatVelocity) const
    {
        floatVelocity = juce::jlimit (0.0f, 1.0f, floatVelocity);
        return getGainMult (floatVelocity, sensitivity.load());
    }
    
    float VelocityHelper::getGainMult (const float floatVelocity, const float floatSensitivity) const
    {
        return juce::jlimit (0.0f, 1.0f,
                             (1.0f - floatVelocity) * (1.0f - floatSensitivity) + floatVelocity);
    }
    
    
}  // namespace midi
    
}  // namespace bav

