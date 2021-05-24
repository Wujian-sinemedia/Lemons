
#pragma once

namespace bav
{
/* Updates the ValueTree with changes from the parameter object */
class ParameterToValueTreeAttachment : private juce::Timer,
                                       public juce::AudioProcessorParameter::Listener
{
public:
    ParameterToValueTreeAttachment (bav::Parameter&    paramToUse,
                                    juce::ValueTree    treeToUse,
                                    juce::UndoManager* um = nullptr);

    virtual ~ParameterToValueTreeAttachment() override;


private:
    void timerCallback() override final;

    void parameterValueChanged (int, float) override final { }
    void parameterGestureChanged (int, bool gestureIsStarting) override final;

    bav::Parameter& param;
    juce::ValueTree tree;

    std::atomic< bool > isChanging;

    juce::CachedValue< float > currentValue;
    juce::CachedValue< float > currentDefaultValue;
    juce::CachedValue< bool >  currentGesture;

    juce::UndoManager* const undoManager;
};


//==============================================================================


/* Updates the parameter object with changes from the ValueTree */
class ValueTreeToParameterAttachment : public juce::ValueTree::Listener
{
public:
    ValueTreeToParameterAttachment (bav::Parameter& paramToUse,
                                    juce::ValueTree treeToUse);


private:
    void valueTreePropertyChanged (juce::ValueTree&,
                                   const juce::Identifier&) override final;

    bav::Parameter& param;
    juce::ValueTree tree;

    bool lastSentChangeState;

    juce::CachedValue< float > currentValue;
    juce::CachedValue< float > currentDefaultValue;
    juce::CachedValue< bool >  currentGesture;
};


//==============================================================================


struct ParameterAttachment : ParameterToValueTreeAttachment,
                             ValueTreeToParameterAttachment
{
    ParameterAttachment (bav::Parameter&    paramToUse,
                         juce::ValueTree    treeToUse,
                         juce::UndoManager* um = nullptr);
};


//===================================================================================================================================
//===================================================================================================================================


static inline void createTwoWayParameterValueTreeAttachments (
    juce::OwnedArray< bav::ParameterAttachment >& attachments,
    juce::ValueTree                               parameterValueTree,
    int                                           totalNumParams,
    std::function< bav::Parameter*(int) >
                       findParameter,
    juce::UndoManager* um                  = nullptr,
    int                paramIndexToStartAt = 0)
{
    jassert (parameterValueTree.isValid());

    attachments.ensureStorageAllocated (totalNumParams);

    for (int i = paramIndexToStartAt; i < paramIndexToStartAt + totalNumParams; ++i)
    {
        auto* parameter = findParameter (i);
        jassert (parameter != nullptr);

        attachments.add (new bav::ParameterAttachment (
            *parameter,
            bav::getChildTreeForParameter (parameterValueTree, parameter),
            um));
    }
}


static inline void createWriteOnlyParameterValueTreeAttachments (
    juce::OwnedArray< bav::ParameterToValueTreeAttachment >& attachments,
    juce::ValueTree                                          parameterValueTree,
    int                                                      totalNumParams,
    std::function< bav::Parameter*(int) >
                       findParameter,
    int                paramIndexToStartAt = 0,
    juce::UndoManager* um                  = nullptr)
{
    jassert (parameterValueTree.isValid());

    attachments.ensureStorageAllocated (totalNumParams);

    for (int i = paramIndexToStartAt; i < paramIndexToStartAt + totalNumParams; ++i)
    {
        auto* parameter = findParameter (i);
        jassert (parameter != nullptr);

        attachments.add (new bav::ParameterToValueTreeAttachment (
            *parameter,
            bav::getChildTreeForParameter (parameterValueTree, parameter),
            um));
    }
}


}  // namespace bav
