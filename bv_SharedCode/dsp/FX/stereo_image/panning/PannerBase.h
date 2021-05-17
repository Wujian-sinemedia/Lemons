/*
 Part of module: bv_SharedCode
 Parent file: bv_SharedCode.h
 */


namespace bav::dsp::FX
{

    
/*
    handy little class that calculates right and left gain values that should be applied to a mono signal in order to place at a desired stereo location.
 
    The stereo location is set using midi pan from 0 to 127 -- 0 being hard left, 64 being center, and 127 being hard right.
 
    Note that this class does no value smoothing, and simply outputs the next target value for the left and right channels each time the panning is changed. You may want to feed this class's output to a juce::SmoothedValue, or employ some other smoothing mechanism.
*/

class MonoToStereoPanner
{
public:
    Panner(): lastRecievedMidiPan(64), leftGain(0.5f), rightGain(0.5f)
    { }
    
    ~Panner()
    { }
    
    void reset()
    {
        leftGain  = 0.5f;
        rightGain = 0.5f;
    }
    
    int getLastMidiPan() const noexcept { return lastRecievedMidiPan; }
    
    float getLeftGain()  const noexcept { return leftGain; }
    
    float getRightGain() const noexcept { return rightGain; }
    
    float getGainMult (const int chan) const
    {
        switch (chan)
        {
            case 0:  return leftGain;
            case 1:  return rightGain;
            default: return 1.0f;
        }
    }
    
    void getGainMults (float& left, float& right)
    {
        left  = leftGain;
        right = rightGain;
    }
    
    void setMidiPan (int newMidiPan)
    {
        jassert (newMidiPan >= 0 && newMidiPan <= 127);
        
        if (lastRecievedMidiPan == newMidiPan)
            return;
        
        const auto panningAngle = juce::jlimit (0.0f, 90.0f,
                                                (90.0f * newMidiPan / 127.0f * juce::MathConstants<float>::pi) / 180.0f);
        
        leftGain  = juce::jlimit (0.0f, 1.0f, std::sin (panningAngle));
        rightGain = juce::jlimit (0.0f, 1.0f, std::cos (panningAngle));
        
        lastRecievedMidiPan = newMidiPan;
    }
    
    void setMidiPan (int newMidiPan,
                     float& leftGainOutput,
                     float& rightGainOutput)
    {
        setMidiPan (newMidiPan);
        leftGainOutput  = leftGain;
        rightGainOutput = rightGain;
    }
    
    
private:
    
    int lastRecievedMidiPan;
    
    float leftGain;
    float rightGain;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Panner)
};
    
    
} // namespace bav

