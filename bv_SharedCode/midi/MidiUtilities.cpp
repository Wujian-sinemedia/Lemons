
namespace bav
{
    
namespace midi
{
    
    bool isMidiNoteBlackKey (const int midipitch)
    {
        jassert (midipitch >= 0 && midipitch <= 127);
        
        switch (midipitch % 12)
        {
            case 1: return true;
            case 3: return true;
            case 6: return true;
            case 8: return true;
            case 10: return true;
            default: return false;
        }
    }
    
    
    bool areNotesSamePitchClass (const int pitch1, const int pitch2)
    {
        jassert (pitch1 >= 0 && pitch2 >= 0 && pitch1 <= 127 && pitch2 <= 127);
        return (pitch1 % 12 == pitch2 % 12);
    }
    
    
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
    
    template<typename SampleType>
    SampleType PitchConverter::mtof (SampleType midiNote) const
    {
        midiNote = juce::jlimit (SampleType(0.0), SampleType(127.0), midiNote);
        constexpr SampleType two = SampleType(2.0);
        return static_cast<SampleType>(concertPitchHz.load() * std::pow(two, ((midiNote - rootNote.load()) / notesPerOctave.load())));
    }
    
    template<typename SampleType>
    SampleType PitchConverter::ftom (const SampleType inputFreq) const
    {
        jassert (inputFreq >= 0);
        return static_cast<SampleType>(notesPerOctave.load() * log2(inputFreq / concertPitchHz.load()) + rootNote.load());
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
    
    float VelocityHelper::intVelocity (int midiVelocity)
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
