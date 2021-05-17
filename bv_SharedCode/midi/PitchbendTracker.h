
namespace bav::midi
{

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
        return getMidifloat (newMidiPitch, lastRecievedPitchbend.load());
    }
    
    float newNoteRecieved (const float newMidiPitch) const
    {
        return getMidifloat (newMidiPitch, lastRecievedPitchbend.load());
    }
    
    float newNoteRecieved (const double newMidiPitch) const
    {
        return getMidifloat (newMidiPitch, lastRecievedPitchbend.load());
    }
    
    void newPitchbendRecieved (const int newPitchbend) noexcept
    {
        lastRecievedPitchbend.store (juce::jlimit (0, 127, newPitchbend));
    }
    
    
private:
    std::atomic<int> rangeUp, rangeDown, lastRecievedPitchbend;
    
    template<typename NoteType>
    float getMidifloat (const NoteType midiPitch, const int pitchbend) const
    {
        if (pitchbend == 64)
            return float(midiPitch);
        
        if (pitchbend > 64)
            return float(((rangeUp.load() * (pitchbend - 65.0f)) / 62.0f) + midiPitch);
        
        const auto currentdownrange = rangeDown.load();
        return float((((1.0f - currentdownrange) * pitchbend) / 63.0f) + midiPitch - currentdownrange);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PitchBendHelper)
};
 
}  // namespace
