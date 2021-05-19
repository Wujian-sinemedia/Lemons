
namespace bav
{
/* Updates the ValueTree with changes from the parameter object */

class FreeStandingParameterToValueTreeAttachment
    : public FreestandingParameter::Listener
{
public:
    FreeStandingParameterToValueTreeAttachment (FreestandingParameter* paramToUse,
                                                juce::ValueTree        treeToUse,
                                                juce::UndoManager*     um = nullptr)
        : param (paramToUse)
        , tree (treeToUse)
        , undoManager (um)
    {
        jassert (tree.isValid());

        param->addListener (this);

        currentValue.referTo (tree, DefaultValueTreeIds::ParameterValue, nullptr);
        currentDefaultValue.referTo (
            tree, DefaultValueTreeIds::ParameterDefaultValue, nullptr);
        currentGesture.referTo (
            tree, DefaultValueTreeIds::ParameterIsChanging, nullptr);
    }

    virtual ~FreeStandingParameterToValueTreeAttachment()
    {
        param->removeListener (this);
    }


    void parameterValueChanged (float newNormalizedValue, float) override final
    {
        if (newNormalizedValue == currentValue.get()) return;

        if (!currentGesture.get()) parameterGestureChanged (true);

        currentValue.setValue (newNormalizedValue, undoManager);
    }

    void parameterDefaultValueChanged (float newNormalizedDefault,
                                       float) override final
    {
        if (newNormalizedDefault == currentDefaultValue.get()) return;

        if (undoManager != nullptr)
        {
            undoManager->beginNewTransaction();
            undoManager->setCurrentTransactionName (
                TRANS ("Changed default value of") + " "
                + param->parameterNameVerbose);
        }

        currentDefaultValue.setValue (newNormalizedDefault, undoManager);
    }

    void parameterGestureChanged (bool gestureIsStarting) override final
    {
        if (gestureIsStarting == currentGesture.get()) return;

        if (gestureIsStarting && undoManager != nullptr)
        {
            undoManager->beginNewTransaction();
            undoManager->setCurrentTransactionName (TRANS ("Changed") + " "
                                                    + param->parameterNameVerbose);
        }

        currentGesture.setValue (gestureIsStarting, undoManager);
    }


private:
    FreestandingParameter* const param;
    juce::ValueTree              tree;

    juce::CachedValue< float > currentValue;
    juce::CachedValue< float > currentDefaultValue;
    juce::CachedValue< bool >  currentGesture;

    juce::UndoManager* const undoManager;
};


//==============================================================================


/* Updates the parameter object with changes from the ValueTree */

class ValueTreeToFreeStandingParameterAttachment : public juce::ValueTree::Listener
{
public:
    ValueTreeToFreeStandingParameterAttachment (FreestandingParameter* paramToUse,
                                                juce::ValueTree        treeToUse)
        : param (paramToUse)
        , tree (treeToUse)
    {
        jassert (tree.isValid());

        tree.addListener (this);

        currentValue.referTo (tree, DefaultValueTreeIds::ParameterValue, nullptr);
        currentDefaultValue.referTo (
            tree, DefaultValueTreeIds::ParameterDefaultValue, nullptr);
        currentGesture.referTo (
            tree, DefaultValueTreeIds::ParameterIsChanging, nullptr);
    }


    void valueTreePropertyChanged (juce::ValueTree&,
                                   const juce::Identifier&) override final
    {
        bool needToEndGesture = false;

        /* Gesture state */
        const auto changeState = currentGesture.get();

        if (changeState != param->isChanging())
        {
            if (changeState) { param->beginChangeGesture(); }
            else
            {
                needToEndGesture = true;
            }
        }

        /* Current parameter value */
        const auto value = currentValue.get();

        if (value != param->getCurrentNormalizedValue())
            param->setValueNormalized (value);

        // if gesture state switched to off, we need to send that message after any value changes
        if (needToEndGesture) param->endChangeGesture();

        /* Parameter default value */
        const auto defaultVal = currentDefaultValue.get();

        if (defaultVal != param->getNormalizedDefault())
        {
            param->setNormalizedDefault (defaultVal);
        }
    }


private:
    FreestandingParameter* const param;
    juce::ValueTree              tree;

    juce::CachedValue< float > currentValue;
    juce::CachedValue< float > currentDefaultValue;
    juce::CachedValue< bool >  currentGesture;
};


//==============================================================================


struct FreeStandingParameterAttachment : FreeStandingParameterToValueTreeAttachment,
                                         ValueTreeToFreeStandingParameterAttachment
{
    FreeStandingParameterAttachment (FreestandingParameter* paramToUse,
                                     juce::ValueTree        treeToUse,
                                     juce::UndoManager*     um = nullptr)

        : FreeStandingParameterToValueTreeAttachment (paramToUse, treeToUse, um)
        , ValueTreeToFreeStandingParameterAttachment (paramToUse, treeToUse)
    {
    }
};


//===================================================================================================================================
//===================================================================================================================================


static inline void createTwoWayFreeParameterValueTreeAttachments (
    juce::OwnedArray< FreeStandingParameterAttachment >& attachments,
    juce::ValueTree                                      parameterValueTree,
    int                                                  totalNumParams,
    std::function< FreestandingParameter*(int) >         findParameter,
    juce::UndoManager*                                   um = nullptr,
    int                                                  paramIndexToStartAt = 0)
{
    jassert (parameterValueTree.isValid());

    attachments.ensureStorageAllocated (totalNumParams);

    for (int i = paramIndexToStartAt; i < paramIndexToStartAt + totalNumParams; ++i)
    {
        auto* parameter = findParameter (i);
        jassert (parameter != nullptr);

        attachments.add (new FreeStandingParameterAttachment (
            parameter,
            bav::getChildTreeForParameter (parameterValueTree, parameter),
            um));
    }
}


static inline void createReadOnlyFreeParameterValueTreeAttachments (
    juce::OwnedArray< ValueTreeToFreeStandingParameterAttachment >& attachments,
    juce::ValueTree                              parameterValueTree,
    int                                          totalNumParams,
    std::function< FreestandingParameter*(int) > findParameter,
    int                                          paramIndexToStartAt = 0)
{
    jassert (parameterValueTree.isValid());

    attachments.ensureStorageAllocated (totalNumParams);

    for (int i = paramIndexToStartAt; i < paramIndexToStartAt + totalNumParams; ++i)
    {
        auto* parameter = findParameter (i);
        jassert (parameter != nullptr);

        attachments.add (new ValueTreeToFreeStandingParameterAttachment (
            parameter,
            bav::getChildTreeForParameter (parameterValueTree, parameter)));
    }
}


} // namespace bav
