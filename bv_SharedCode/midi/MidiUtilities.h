/*
 Part of module: bv_SharedCode
 Parent file: bv_SharedCode.h
*/



namespace bav

{
    
namespace midi
{
    
    
    bool isMidiNoteBlackKey (const int midipitch);
    
    bool areNotesSamePitchClass (const int pitch1, const int pitch2);
    
    
    void copyRangeOfMidiBuffer (const juce::MidiBuffer& readingBuffer, juce::MidiBuffer& destBuffer,
                                const int startSampleOfInput,
                                const int startSampleOfOutput,
                                const int numSamples);
    
    
    class PitchConverter
    {
    public:
        PitchConverter(const int initialConcertPitch, const int initialRootNote, const int initialNotesPerOctave);
        
        // converts midi pitch to frequency in Hz
        int mtof (int midiNote) const;
        float mtof (float midiNote) const;
        
        // converts frequency in Hz to midipitch
        int ftom (const int inputFreq) const;
        float ftom (const float inputFreq) const;
        
        void setConcertPitchHz (const int newConcertPitch) noexcept;
        
        int getCurrentConcertPitchHz() const noexcept { return concertPitchHz.load(); }
        
        void setNotesPerOctave (const int newNPO) noexcept;
        
        int getCurrentNotesPerOctave() const noexcept { return notesPerOctave.load(); }
        
        void setRootNote (const int newRoot) noexcept;
        
        int getCurrentRootNote() const noexcept { return rootNote.load(); }
        
    private:
        std::atomic<int> concertPitchHz; // the frequency in Hz of the root note. Usually 440 in standard Western tuning.
        
        std::atomic<int> rootNote; // the midiPitch that corresponds to concertPitchHz. Usually 69 (A4) in Western standard tuning.
        
        std::atomic<int> notesPerOctave; // the number of notes per octave. Usually 12 in standard Western tuning.
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchConverter)
        
    };
    
    
    
    class PitchBendHelper
    {
    public:
        PitchBendHelper(const int initialStUp, const int initialStDwn);
        
        void setRange (const int newStUp, const int newStDown) noexcept;
        
        int getCurrentRangeUp()   const noexcept { return rangeUp.load(); }
        int getCurrentRangeDown() const noexcept { return rangeDown.load(); }
        
        int getLastRecievedPitchbend() const noexcept { return lastRecievedPitchbend.load(); }
        
        float newNoteRecieved (const int newMidiPitch) const;
        
        void newPitchbendRecieved (const int newPitchbend) noexcept;
        
    private:
        std::atomic<int> rangeUp, rangeDown, lastRecievedPitchbend;
        
        float getMidifloat (const int midiPitch, const int pitchbend) const;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchBendHelper)
    };
    
    
    
    class VelocityHelper
    {
    public:
        VelocityHelper(const int initialSensitivity);
        
        void setSensitivity (int newSensitivity) noexcept;
        
        void setFloatSensitivity (const float newSensitivity) noexcept;
        
        float getCurrentSensitivity() const noexcept { return sensitivity.load(); }
        
        float intVelocity (int midiVelocity) const;
        
        float floatVelocity (float floatVelocity) const;
        
    private:
        
        std::atomic<float> sensitivity;
        
        float getGainMult (const float floatVelocity, const float floatSensitivity) const;
        
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VelocityHelper)
    };
    
    
}  // namespace midi

} // namespace bav

