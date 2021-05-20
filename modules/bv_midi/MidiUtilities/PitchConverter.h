
namespace bav::midi
{
// helper class to convert midi pitch to frequency and vice versa, with tracking for alternate concert pitch Hz, root note, and number of notes per octave.
class PitchConverter
{
public:
    PitchConverter (const int initialConcertPitch,
                    const int initialRootNote,
                    const int initialNotesPerOctave)
        : concertPitchHz (initialConcertPitch), rootNote (initialRootNote), notesPerOctave (initialNotesPerOctave)
    {
    }

    // converts midi pitch to frequency in Hz
    int mtof (int midiNote) const
    {
        jassert (midiNote >= 0 && midiNote <= 127);
        return juce::roundToInt (
            concertPitchHz.load()
            * std::pow (2, ((midiNote - rootNote.load()) / notesPerOctave.load())));
    }

    float mtof (float midiNote) const
    {
        jassert (midiNote >= 0.0f && midiNote <= 127.0f);
        return float (
            concertPitchHz.load()
            * std::pow (2.0f,
                        ((midiNote - rootNote.load()) / notesPerOctave.load())));
    }

    // converts frequency in Hz to midipitch
    int ftom (const int inputFreq) const
    {
        jassert (inputFreq > 0);
        return juce::roundToInt (notesPerOctave.load()
                                     * log2 (inputFreq / concertPitchHz.load())
                                 + rootNote.load());
    }

    float ftom (const float inputFreq) const
    {
        jassert (inputFreq > 0.0f);
        return float (notesPerOctave.load()
                          * log2 (inputFreq / concertPitchHz.load())
                      + rootNote.load());
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
    std::atomic< int >
        concertPitchHz;  // the frequency in Hz of the root note. Usually 440 in standard Western tuning.

    std::atomic< int >
        rootNote;  // the midiPitch that corresponds to concertPitchHz. Usually 69 (A4) in Western standard tuning.

    std::atomic< int >
        notesPerOctave;  // the number of notes per octave. Usually 12 in standard Western tuning.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchConverter)
};

}  // namespace bav::midi
