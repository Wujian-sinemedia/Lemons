
namespace bav
{


/* Updates the ValueTree with changes from the parameter object */
class ParameterToValueTreeAttachment   :     private juce::Timer,
                                             public  juce::AudioProcessorParameter::Listener
{
public:
    ParameterToValueTreeAttachment (bav::Parameter* paramToUse,
                                    juce::ValueTree treeToUse,
                                    juce::UndoManager* um = nullptr)
    : param (paramToUse),
    tree (treeToUse),
    undoManager (um)
    {
        jassert (tree.isValid());
        
        param->orig()->addListener (this);
        startTimerHz (60);
        isChanging.store (false);
        
        currentValue.referTo (tree, DefaultValueTreeIds::ParameterValue, nullptr);
        currentDefaultValue.referTo (tree, DefaultValueTreeIds::ParameterDefaultValue, nullptr);
        currentGesture.referTo (tree, DefaultValueTreeIds::ParameterIsChanging, nullptr);
    }
    
    virtual ~ParameterToValueTreeAttachment() override
    {
        stopTimer();
        param->orig()->removeListener (this);
    }
    
    
    void timerCallback() override final
    {
        bool needToEndGesture = false;
        
        /* Gesture state */
        const auto changeState = isChanging.load();
        
        if (currentGesture.get() != changeState)
        {
            if (changeState)
            {
                if (undoManager != nullptr)
                {
                    undoManager->beginNewTransaction();
                    undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + param->parameterNameVerbose);
                }
                
                currentGesture.setValue (true, nullptr);
            }
            else
            {
                needToEndGesture = true;
            }
        }
        
        /* Current parameter value */
        const auto newValue = param->getCurrentDenormalizedValue();
        
        if (currentValue.get() != newValue)
            currentValue.setValue (newValue, nullptr);
        
        // if gesture state switched to off, we need to send that message after any value changes
        if (needToEndGesture)
            currentGesture.setValue (false, nullptr);
        
        
        /* Parameter default value */
        const auto newDefault = param->getNormalizedDefault();
        
        if (currentDefaultValue.get() != newDefault)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + param->parameterNameVerbose);
            }
            
            currentDefaultValue.setValue (newDefault, nullptr);
        }
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
    juce::CachedValue<float> currentDefaultValue;
    juce::CachedValue<bool>  currentGesture;
    
    juce::UndoManager* const undoManager;
};


//==============================================================================


/* Updates the parameter object with changes from the ValueTree */
class ValueTreeToParameterAttachment   :    public juce::ValueTree::Listener
{
public:
    ValueTreeToParameterAttachment (bav::Parameter* paramToUse,
                                    juce::ValueTree treeToUse,
                                    juce::UndoManager* um = nullptr)
    : param (paramToUse),
    tree (treeToUse),
    undoManager (um)
    {
        jassert (tree.isValid());
        
        tree.addListener (this);
        lastSentChangeState = false;
        
        currentValue.referTo (tree, DefaultValueTreeIds::ParameterValue, nullptr);
        currentDefaultValue.referTo (tree, DefaultValueTreeIds::ParameterDefaultValue, nullptr);
        currentGesture.referTo (tree, DefaultValueTreeIds::ParameterIsChanging, nullptr);
    }
    
    
    void valueTreePropertyChanged (juce::ValueTree&, const juce::Identifier&) override final
    {
        bool needToEndGesture = false;
        
        /* Gesture state */
        const auto changeState = currentGesture.get();
        
        if (changeState != lastSentChangeState)
        {
            lastSentChangeState = changeState;
            
            if (changeState)
            {
                if (undoManager != nullptr)
                {
                    undoManager->beginNewTransaction();
                    undoManager->setCurrentTransactionName (TRANS ("Changed") + " " + param->parameterNameVerbose);
                }
                
                param->orig()->beginChangeGesture();
            }
            else
            {
                needToEndGesture = true;
            }
        }
        
        /* Current parameter value */
        const auto value = currentValue.get();
        
        if (value != param->getCurrentDenormalizedValue())
            param->orig()->setValueNotifyingHost (value);
        
        // if gesture state switched to off, we need to send that message after any value changes
        if (needToEndGesture)
            param->orig()->endChangeGesture();
        
        
        /* Parameter default value */
        const auto defaultVal = currentDefaultValue.get();
        
        if (defaultVal != param->getNormalizedDefault())
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + param->parameterNameVerbose);
            }
            
            param->setNormalizedDefault (defaultVal);
        }
    }
    
    
private:
    bav::Parameter* const param;
    juce::ValueTree tree;
    
    bool lastSentChangeState;
    
    juce::CachedValue<float> currentValue;
    juce::CachedValue<float> currentDefaultValue;
    juce::CachedValue<bool>  currentGesture;
    
    juce::UndoManager* const undoManager;
};


//==============================================================================


struct ParameterAttachment  :   ParameterToValueTreeAttachment,
ValueTreeToParameterAttachment
{
    ParameterAttachment (bav::Parameter* paramToUse,
                         juce::ValueTree treeToUse,
                         juce::UndoManager* um = nullptr)
    
    : ParameterToValueTreeAttachment (paramToUse, treeToUse, nullptr),
    ValueTreeToParameterAttachment (paramToUse, treeToUse, um)  // only use one UndoManager at a time...
    { }
};


}  // namespace
