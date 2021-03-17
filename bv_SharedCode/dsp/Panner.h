/*
 Part of module: bv_SharedCode
 Parent file: bv_SharedCode.h
 */


namespace bav
{

namespace dsp
{
    

class Panner
{
public:
    Panner(): lastRecievedMidiPan(64), leftGain(0.5f), rightGain(0.5f), prevLeftGain(0.5f), prevRightGain(0.5f)
    { }
    
    ~Panner()
    { }
    
    void reset()
    {
        left  = 0.5f;
        right = 0.5f;
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
        newMidiPan = juce::jlimit (0, 127, newMidiPan);
        
        if (lastRecievedMidiPan == newMidiPan)
            return;
        
        auto panningAngle = (90.0f * newMidiPan / 127.0f * juce::MathConstants<float>::pi) / 180.0f;
        
        panningAngle = juce::jlimit (0.0f, 90.0f, panningAngle);
        
        leftGain  = std::sin (panningAngle);
        rightGain = std::cos (panningAngle);
        
        jassert (leftGain >= 0.0f && leftGain <= 1.0f);
        jassert (rightGain >= 0.0f && rightGain <= 1.0f);
        
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
    
    
}  // namespace dsp

} // namespace bav

