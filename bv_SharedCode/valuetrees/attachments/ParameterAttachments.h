
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
                
                currentGesture.setValue (true, undoManager);
            }
            else
            {
                needToEndGesture = true;
            }
        }
        
        /* Current parameter value */
        const auto newValue = param->getCurrentNormalizedValue();
        
        if (currentValue.get() != newValue)
            currentValue.setValue (newValue, undoManager);
        
        // if gesture state switched to off, we need to send that message after any value changes
        if (needToEndGesture)
            currentGesture.setValue (false, undoManager);
        
        
        /* Parameter default value */
        const auto newDefault = param->getNormalizedDefault();
        
        if (currentDefaultValue.get() != newDefault)
        {
            if (undoManager != nullptr)
            {
                undoManager->beginNewTransaction();
                undoManager->setCurrentTransactionName (TRANS ("Changed default value of") + " " + param->parameterNameVerbose);
            }
            
            currentDefaultValue.setValue (newDefault, undoManager);
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
                                    juce::ValueTree treeToUse)
      : param (paramToUse),
        tree (treeToUse)
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
};


//==============================================================================


struct ParameterAttachment  :   ParameterToValueTreeAttachment,
ValueTreeToParameterAttachment
{
    ParameterAttachment (bav::Parameter* paramToUse,
                         juce::ValueTree treeToUse,
                         juce::UndoManager* um = nullptr)
    
      : ParameterToValueTreeAttachment (paramToUse, treeToUse, um),
        ValueTreeToParameterAttachment (paramToUse, treeToUse)
    { }
};


//===================================================================================================================================
//===================================================================================================================================


static inline void createTwoWayParameterValueTreeAttachments (juce::OwnedArray<bav::ParameterAttachment>& attachments,
                                                              juce::ValueTree parameterValueTree,
                                                              int totalNumParams,
                                                              std::function< bav::Parameter* (int) > findParameter,
                                                              juce::UndoManager* um = nullptr,
                                                              int paramIndexToStartAt = 0)
{
    jassert (parameterValueTree.isValid());
    
    attachments.ensureStorageAllocated (totalNumParams);
    
    for (int i = paramIndexToStartAt;
         i < paramIndexToStartAt + totalNumParams;
         ++i)
    {
        auto* parameter = findParameter (i);
        jassert (parameter != nullptr);
        
        attachments.add (new bav::ParameterAttachment (parameter,
                                                       bav::getChildTreeForParameter (parameterValueTree, parameter),
                                                       um));
    }
}


static inline void createReadOnlyParameterValueTreeAttachments (juce::OwnedArray<bav::ValueTreeToParameterAttachment>& attachments,
                                                                juce::ValueTree parameterValueTree,
                                                                int totalNumParams,
                                                                std::function< bav::Parameter* (int) > findParameter,
                                                                int paramIndexToStartAt = 0)
{
    jassert (parameterValueTree.isValid());
    
    attachments.ensureStorageAllocated (totalNumParams);
    
    for (int i = paramIndexToStartAt;
         i < paramIndexToStartAt + totalNumParams;
         ++i)
    {
        auto* parameter = findParameter (i);
        jassert (parameter != nullptr);
        
        attachments.add (new bav::ValueTreeToParameterAttachment (parameter,
                                                                  bav::getChildTreeForParameter (parameterValueTree, parameter)));
    }
}


static inline void createWriteOnlyParameterValueTreeAttachments (juce::OwnedArray<bav::ParameterToValueTreeAttachment>& attachments,
                                                                 juce::ValueTree parameterValueTree,
                                                                 int totalNumParams,
                                                                 std::function< bav::Parameter* (int) > findParameter,
                                                                 int paramIndexToStartAt = 0,
                                                                 juce::UndoManager* um = nullptr)
{
    jassert (parameterValueTree.isValid());
    
    attachments.ensureStorageAllocated (totalNumParams);
    
    for (int i = paramIndexToStartAt;
         i < paramIndexToStartAt + totalNumParams;
         ++i)
    {
        auto* parameter = findParameter (i);
        jassert (parameter != nullptr);
        
        attachments.add (new bav::ParameterToValueTreeAttachment (parameter,
                                                                  bav::getChildTreeForParameter (parameterValueTree, parameter),
                                                                  um));
    }
}


}  // namespace
