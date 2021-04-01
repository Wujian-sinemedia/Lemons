/*
 Part of module: bv_SharedCode
 Parent file: bv_SharedCode.h
*/



namespace bav::midi
{
    
    
    // returns true if the given midinote is a black key
    template<typename noteType>
    inline bool isMidiNoteBlackKey (noteType midipitch)
    {
        jassert (midipitch >= noteType(0) && midipitch <= noteType(127));
        
        switch (midipitch % 12)
        {
            case 1:  return true;
            case 3:  return true;
            case 6:  return true;
            case 8:  return true;
            case 10: return true;
            default: return false;
        }
    }
    
    
    // returns true if the two pitches are the same pitch class, regardless of octave
    template<typename noteType1, typename noteType2>
    inline bool areNotesSamePitchClass (const noteType1 pitch1, const noteType2 pitch2)
    {
        jassert (pitch1 >= noteType1(0) && pitch2 >= noteType2(0) && pitch1 <= noteType1(127) && pitch2 <= noteType2(127));
        return (pitch1 % 12 == pitch2 % 12);
    }
    
    
    // copies a range of events from one juce::MidiBuffer to another, optionally providing a sample offset
    inline void copyRangeOfMidiBuffer (const juce::MidiBuffer& readingBuffer, juce::MidiBuffer& destBuffer,
                                       const int startSampleOfInput,
                                       const int startSampleOfOutput,
                                       const int numSamples)
    {
        destBuffer.clear (startSampleOfOutput, numSamples);
        destBuffer.addEvents (readingBuffer, startSampleOfInput, numSamples, startSampleOfOutput - startSampleOfInput);
    }
    
    
    /*
    */
    
    
    // helper class to convert midi pitch to frequency and vice versa, with tracking for alternate concert pitch Hz, root note, and number of notes per octave.
    class PitchConverter
    {
    public:
        PitchConverter(const int initialConcertPitch, const int initialRootNote, const int initialNotesPerOctave):
            concertPitchHz(initialConcertPitch),
            rootNote(initialRootNote),
            notesPerOctave(initialNotesPerOctave)
        { }
        
        // converts midi pitch to frequency in Hz
        int mtof (int midiNote) const
        {
            jassert (midiNote >= 0 && midiNote <= 127);
            return juce::roundToInt(concertPitchHz.load() * std::pow(2, ((midiNote - rootNote.load()) / notesPerOctave.load())));
        }
        
        float mtof (float midiNote) const
        {
            jassert (midiNote >= 0.0f && midiNote <= 127.0f);
            return float(concertPitchHz.load() * std::pow(2.0f, ((midiNote - rootNote.load()) / notesPerOctave.load())));
        }
        
        // converts frequency in Hz to midipitch
        int ftom (const int inputFreq) const
        {
            jassert (inputFreq > 0);
            return juce::roundToInt(notesPerOctave.load() * log2(inputFreq / concertPitchHz.load()) + rootNote.load());
        }
        
        float ftom (const float inputFreq) const
        {
            jassert (inputFreq > 0.0f);
            return float(notesPerOctave.load() * log2(inputFreq / concertPitchHz.load()) + rootNote.load());
        }
        
        void setConcertPitchHz (const int newConcertPitch) noexcept
        {
            jassert (newConcertPitch > 0);
            concertPitchHz.store (newConcertPitch);
        }
        
        int getCurrentConcertPitchHz() const noexcept { return concertPitchHz.load(); }
        
        void setNotesPerOctave (const int newNPO) noexcept
        {
            jassert (newNPO > 0);
            notesPerOctave.store (newNPO);
        }
        
        int getCurrentNotesPerOctave() const noexcept { return notesPerOctave.load(); }
        
        void setRootNote (const int newRoot) noexcept
        {
            jassert (newRoot > 0);
            rootNote.store (newRoot);
        }
        
        int getCurrentRootNote() const noexcept { return rootNote.load(); }
        
    private:
        std::atomic<int> concertPitchHz; // the frequency in Hz of the root note. Usually 440 in standard Western tuning.
        
        std::atomic<int> rootNote; // the midiPitch that corresponds to concertPitchHz. Usually 69 (A4) in Western standard tuning.
        
        std::atomic<int> notesPerOctave; // the number of notes per octave. Usually 12 in standard Western tuning.
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchConverter)
        
    };
    
    
    /*
    */
    
    
    // helper class for outputting midifloat values based on input midiPitch, pitchbend, and pitchbend range controls
    class PitchBendHelper
    {
    public:
        PitchBendHelper(const int initialStUp, const int initialStDwn):
            rangeUp(initialStUp),
            rangeDown(initialStDwn),
            lastRecievedPitchbend(64)
        { }
        
        void setRange (const int newStUp, const int newStDown) noexcept
        {
            rangeUp.store (newStUp);
            rangeDown.store (newStDown);
        }
        
        int getCurrentRangeUp()   const noexcept { return rangeUp.load(); }
        int getCurrentRangeDown() const noexcept { return rangeDown.load(); }
        
        int getLastRecievedPitchbend() const noexcept { return lastRecievedPitchbend.load(); }
        
        float newNoteRecieved (const int newMidiPitch) const
        {
            return getMidifloat (juce::jlimit (0, 127, newMidiPitch),
                                 lastRecievedPitchbend.load());
        }
        
        void newPitchbendRecieved (const int newPitchbend) noexcept
        {
            lastRecievedPitchbend.store (juce::jlimit (0, 127, newPitchbend));
        }
        
        
    private:
        std::atomic<int> rangeUp, rangeDown, lastRecievedPitchbend;
        
        float getMidifloat (const int midiPitch, const int pitchbend) const
        {
            jassert (juce::isPositiveAndBelow(midiPitch, 128) && juce::isPositiveAndBelow(pitchbend, 128));
            
            if (pitchbend == 64)
                return float(midiPitch);
            
            if (pitchbend > 64)
                return float(((rangeUp.load() * (pitchbend - 65)) / 62) + midiPitch);
            
            const auto currentdownrange = rangeDown.load();
            return float((((1 - currentdownrange) * pitchbend) / 63) + midiPitch - currentdownrange);
        }
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchBendHelper)
    };
    
    
    /*
    */
    
    
    // helper class for outputting weighted midi velocities
    class VelocityHelper
    {
    public:
        VelocityHelper(const int initialSensitivity): sensitivity(initialSensitivity/100.0f) { }
        
        void setSensitivity (int newSensitivity) noexcept
        {
            jassert (newSensitivity >= 0 && newSensitivity <= 100);
            sensitivity.store (newSensitivity * 0.01f);
        }
        
        void setFloatSensitivity (const float newSensitivity) noexcept
        {
            jassert (newSensitivity >= 0.0f && newSensitivity <= 1.0f);
            sensitivity.store (newSensitivity);
        }
        
        float getCurrentSensitivity() const noexcept { return sensitivity.load(); }
        
        float intVelocity (int midiVelocity) const
        {
            jassert (midiVelocity >= 0 && midiVelocity <= 127);
            constexpr auto inv127 = 1.0f / 127.0f;
            return getGainMult (midiVelocity * inv127, sensitivity.load());
        }
        
        float floatVelocity (float floatVelocity) const
        {
            jassert (floatVelocity >= 0.0f && floatVelocity <= 1.0f);
            return getGainMult (floatVelocity, sensitivity.load());
        }
        
        
    private:
        
        std::atomic<float> sensitivity;
        
        float getGainMult (const float floatVelocity, const float floatSensitivity) const
        {
            const auto gain = (1.0f - floatVelocity) * (1.0f - floatSensitivity) + floatVelocity;
            
            jassert (gain >= 0.0f && gain <= 1.0f);
            
            return gain;
        }
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VelocityHelper)
    };
    
    
} // namespace 

