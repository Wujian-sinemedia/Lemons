
namespace bav
{
ParameterToValueTreeAttachment::ParameterToValueTreeAttachment (bav::Parameter&    paramToUse,
                                                                juce::ValueTree    treeToUse,
                                                                juce::UndoManager* um)
    : param (paramToUse), tree (treeToUse), undoManager (um)
{
    jassert (tree.isValid());

    param.rap.addListener (this);
    startTimerHz (60);
    isChanging.store (false);

    currentValue.referTo (tree, DefaultValueTreeIds::ParameterValue, nullptr);
    currentDefaultValue.referTo (
        tree, DefaultValueTreeIds::ParameterDefaultValue, nullptr);
    currentGesture.referTo (
        tree, DefaultValueTreeIds::ParameterIsChanging, nullptr);
}

ParameterToValueTreeAttachment::~ParameterToValueTreeAttachment()
{
    stopTimer();
    param.rap.removeListener (this);
}

void ParameterToValueTreeAttachment::timerCallback()
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
                undoManager->setCurrentTransactionName (
                    TRANS ("Changed") + " " + param.parameterNameVerbose);
            }

            currentGesture.setValue (true, undoManager);
        }
        else
        {
            needToEndGesture = true;
        }
    }

    /* Current parameter value */
    const auto newValue = param.getCurrentNormalizedValue();

    if (currentValue.get() != newValue)
        currentValue.setValue (newValue, undoManager);

    // if gesture state switched to off, we need to send that message after any value changes
    if (needToEndGesture) currentGesture.setValue (false, undoManager);


    /* Parameter default value */
    const auto newDefault = param.getNormalizedDefault();

    if (currentDefaultValue.get() != newDefault)
    {
        if (undoManager != nullptr)
        {
            undoManager->beginNewTransaction();
            undoManager->setCurrentTransactionName (
                TRANS ("Changed default value of") + " "
                + param.parameterNameVerbose);
        }

        currentDefaultValue.setValue (newDefault, undoManager);
    }
}

void ParameterToValueTreeAttachment::parameterGestureChanged (int, bool gestureIsStarting)
{
    isChanging.store (gestureIsStarting);
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

ValueTreeToParameterAttachment::ValueTreeToParameterAttachment (bav::Parameter& paramToUse,
                                                                juce::ValueTree treeToUse)
    : param (paramToUse), tree (treeToUse)
{
    jassert (tree.isValid());

    tree.addListener (this);
    lastSentChangeState = false;

    currentValue.referTo (tree, DefaultValueTreeIds::ParameterValue, nullptr);
    currentDefaultValue.referTo (
        tree, DefaultValueTreeIds::ParameterDefaultValue, nullptr);
    currentGesture.referTo (
        tree, DefaultValueTreeIds::ParameterIsChanging, nullptr);
}

void ValueTreeToParameterAttachment::valueTreePropertyChanged (juce::ValueTree&,
                                                               const juce::Identifier&)
{
    bool needToEndGesture = false;

    /* Gesture state */
    const auto changeState = currentGesture.get();

    if (changeState != lastSentChangeState)
    {
        lastSentChangeState = changeState;

        if (changeState)
        {
            param.rap.beginChangeGesture();
        }
        else
        {
            needToEndGesture = true;
        }
    }

    /* Current parameter value */
    const auto value = currentValue.get();

    if (value != param.getCurrentDenormalizedValue())
        param.rap.setValueNotifyingHost (value);

    // if gesture state switched to off, we need to send that message after any value changes
    if (needToEndGesture) param.rap.endChangeGesture();


    /* Parameter default value */
    const auto defaultVal = currentDefaultValue.get();

    if (defaultVal != param.getNormalizedDefault())
    {
        param.setNormalizedDefault (defaultVal);
    }
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

ParameterAttachment::ParameterAttachment (bav::Parameter&    paramToUse,
                                          juce::ValueTree    treeToUse,
                                          juce::UndoManager* um)

    : ParameterToValueTreeAttachment (paramToUse, treeToUse, um), ValueTreeToParameterAttachment (paramToUse, treeToUse)
{
}


}  // namespace bav
