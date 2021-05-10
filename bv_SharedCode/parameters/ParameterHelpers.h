

namespace bav
{


static inline float getNormalizedParameterValue (juce::RangedAudioParameter& param)
{
    return param.getValue();
}



static inline juce::AudioProcessorParameterGroup* findParameterSubgroup (const juce::AudioProcessorParameterGroup* parameterTree,
                                                                         const juce::String& subgroupName)
{
    for (auto* node : *parameterTree)
        if (auto* group = node->getGroup())
            if (group->getName() == subgroupName)
                return group;
    
    return nullptr;
}

//==============================================================================
//==============================================================================


/* Updates the ValueTree with changes from the parameter object */
class ParameterToValueTreeAttachment   :     private juce::Timer,
                                             public  juce::AudioProcessorParameter::Listener
{
public:
    ParameterToValueTreeAttachment (bav::Parameter* paramToUse,
                                    juce::ValueTree treeToUse)
      : param (paramToUse),
        tree (treeToUse)
    {
        jassert (tree.isValid());
        
        param->orig()->addListener (this);
        startTimerHz (10);
        isChanging.store (false);
        
        currentValue.referTo (tree, param->identifier, nullptr);
        currentGesture.referTo (tree, param->gestureIdentifier, nullptr);
    }
    
    virtual ~ParameterToValueTreeAttachment() override
    {
        stopTimer();
        param->orig()->removeListener (this);
    }
    
    void timerCallback() override final
    {
        const auto newValue = param->getCurrentDenormalizedValue();
        
        if (currentValue.get() != newValue)
            currentValue.setValue (newValue, nullptr);
        
        const auto changeState = isChanging.load();
        
        if (currentGesture.get() != changeState)
            currentGesture.setValue (changeState, nullptr);
    }
    
    void parameterValueChanged (int, float) override final { }
    
    void parameterGestureChanged (int, bool gestureIsStarting) override final
    {
        isChanging.store (gestureIsStarting);
    }
    
    
private:
    bav::Parameter* const param;
    juce::ValueTree tree;
    
    std::atomic<bool> isChanging;
    
    juce::CachedValue<float> currentValue;
    juce::CachedValue<bool>  currentGesture;
};


/* Updates the parameter object with changes from the ValueTree */
class ValueTreeToParameterAttachment   :    public juce::ValueTree::Listener
{
public:
    ValueTreeToParameterAttachment (bav::Parameter* paramToUse,
                                    juce::ValueTree treeToUse)
      : param (paramToUse),
        tree (treeToUse)
    {
        jassert (tree.isValid());
        
        tree.addListener (this);
        lastSentChangeState = false;
        
        currentValue.referTo (tree, param->identifier, nullptr);
        currentGesture.referTo (tree, param->gestureIdentifier, nullptr);
    }

    
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override final
    {
        const auto value = currentValue.get();
        
        if (value != param->getCurrentDenormalizedValue())
            param->orig()->setValueNotifyingHost (value);
        
        const auto changing = currentGesture.get();
        
        if (changing != lastSentChangeState)
        {
            lastSentChangeState = changing;
            
            if (changing)
                param->orig()->beginChangeGesture();
            else
                param->orig()->endChangeGesture();
        }
    }
    
    
private:
    bav::Parameter* const param;
    juce::ValueTree tree;
    
    bool lastSentChangeState;
    
    juce::CachedValue<float> currentValue;
    juce::CachedValue<bool>  currentGesture;
};



struct ParameterAttachment  :   ParameterToValueTreeAttachment,
                                ValueTreeToParameterAttachment
{
    ParameterAttachment (bav::Parameter* paramToUse,
                         juce::ValueTree treeToUse)
        : ParameterToValueTreeAttachment (paramToUse, treeToUse),
          ValueTreeToParameterAttachment (paramToUse, treeToUse)
    { }
};


}  // namespace
