

namespace bav
{


static inline juce::AudioProcessorParameterGroup* findParameterSubgroup (const juce::AudioProcessorParameterGroup* parameterTree,
                                                                         const juce::String& subgroupName)
{
    jassert (parameterTree != nullptr);
    
    for (auto* node : *parameterTree)
        if (auto* group = node->getGroup())
            if (group->getName() == subgroupName)
                return group;
    
    return nullptr;
}


//==============================================================================
//==============================================================================


static inline void parseParameterTreeForParameterPointers (const juce::AudioProcessorParameterGroup* group,
                                                           std::vector< bav::Parameter* >& pointers)
{
    jassert (group != nullptr);
    
    for (auto* node : *group)
    {
        if (auto* rawParam = node->getParameter())
        {
            if (auto* parameter = dynamic_cast<bav::Parameter*> (rawParam))
                pointers.push_back (parameter);
            else
                jassertfalse;
        }
        else if (auto* thisGroup = node->getGroup())
        {
            parseParameterTreeForParameterPointers (thisGroup, pointers);
        }
    }
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
        
        currentValue.referTo (tree, DefaultValueTreeIds::ParameterValue, nullptr);
        currentGesture.referTo (tree, DefaultValueTreeIds::ParameterIsChanging, nullptr);
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


//==============================================================================


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
        
        currentValue.referTo (tree, DefaultValueTreeIds::ParameterValue, nullptr);
        currentGesture.referTo (tree, DefaultValueTreeIds::ParameterIsChanging, nullptr);
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


//==============================================================================


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
