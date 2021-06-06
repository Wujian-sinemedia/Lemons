
namespace bav
{
Parameter::ValueChangeAction::ValueChangeAction (Parameter& p, float newValue, float prevVal)
    : param (p), targetValue (newValue), prevValue (prevVal)
{
}

bool Parameter::ValueChangeAction::perform()
{
    if (param.getCurrentNormalizedValue() == targetValue)
        return false;

    param.setValueInternal (targetValue);
    return true;
}

bool Parameter::ValueChangeAction::undo()
{
    if (param.getCurrentNormalizedValue() == prevValue)
        return false;

    param.setValueInternal (prevValue);
    return true;
}

juce::UndoableAction* Parameter::ValueChangeAction::createCoalescedAction (UndoableAction* nextAction)
{
    if (auto* other = dynamic_cast< ValueChangeAction* > (nextAction))
    {
        if (other->param == param)
        {
            return new ValueChangeAction (param, other->targetValue, prevValue);
        }
    }

    return nullptr;
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

Parameter::DefaultChangeAction::DefaultChangeAction (Parameter& p, float newNormalizedDefault, float prevNormDefault)
    : param (p), targetDefault (newNormalizedDefault), prevDefault (prevNormDefault)
{
}

bool Parameter::DefaultChangeAction::perform()
{
    if (param.getNormalizedDefault() == targetDefault)
        return false;

    param.setDefaultInternal (targetDefault);
    return true;
}

bool Parameter::DefaultChangeAction::undo()
{
    if (param.getNormalizedDefault() == prevDefault)
        return false;

    param.setDefaultInternal (prevDefault);
    return true;
}

juce::UndoableAction* Parameter::DefaultChangeAction::createCoalescedAction (UndoableAction* nextAction)
{
    if (auto* other = dynamic_cast< DefaultChangeAction* > (nextAction))
    {
        if (other->param == param)
        {
            return new DefaultChangeAction (param, other->targetDefault, prevDefault);
        }
    }

    return nullptr;
}

/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

Parameter::Parameter (RangedParam& p,
                      String       paramNameShort,
                      String       paramNameVerbose,
                      bool         isAutomatable,
                      bool         metaParam)
    : SerializableData (paramNameVerbose),
      rap (p),
      parameterNameShort (TRANS (paramNameShort)),
      parameterNameVerbose (TRANS (paramNameVerbose)),
      automatable (isAutomatable),
      metaParameter (metaParam),
      valueChangeTransactionName (TRANS ("Changed") + " " + parameterNameVerbose),
      defaultChangeTransactionName (TRANS ("Changed default value of") + " " + parameterNameVerbose)
{
    currentDefault.store (rap.getDefaultValue());
    lastActionedValue = currentDefault;
}

bool Parameter::operator== (const Parameter& other)
{
    return dataIdentifier == other.dataIdentifier;
}

int Parameter::getMidiControllerNumber() const
{
    return midiControllerNumber.load();
}

bool Parameter::isMidiControllerMapped() const
{
    return getMidiControllerNumber() > -1;
}

void Parameter::setMidiControllerNumber (int newControllerNumber)
{
    midiControllerNumber.store (newControllerNumber);
}

void Parameter::resetMidiControllerMapping()
{
    setMidiControllerNumber (-1);
}

void Parameter::processNewControllerMessage (int controllerNumber, int controllerValue, bool triggerAction)
{
    if (controllerNumber == getMidiControllerNumber())
    {
        const auto& range = rap.getNormalisableRange();
        setDenormalizedValue (juce::jmap (static_cast< float > (controllerValue),
                                          0.f, 127.f,
                                          range.start, range.end));
        
        if (triggerAction)
            doAction();
    }
}

void Parameter::beginGesture()
{
    if (isChanging())
        return;

    //    if (um != nullptr)
    //    {
    //        um->beginNewTransaction (valueChangeTransactionName);
    //    }

    changing.store (true);
    rap.beginChangeGesture();

    listeners.call ([] (Listener& l)
                    { l.parameterGestureStateChanged (true); });
}

void Parameter::endGesture()
{
    if (! isChanging())
        return;

    changing.store (false);
    rap.endChangeGesture();

    listeners.call ([] (Listener& l)
                    { l.parameterGestureStateChanged (false); });
}

bool Parameter::isChanging() const
{
    return changing.load();
}

float Parameter::getNormalizedDefault() const
{
    return currentDefault.load();
}

float Parameter::getDenormalizedDefault() const
{
    return denormalize (getNormalizedDefault());
}

void Parameter::setNormalizedDefault (float value)
{
    jassert (value >= 0.0f && value <= 1.0f);

    if (value == getNormalizedDefault()) return;

    setDefaultInternal (value);

    //    if (um != nullptr)
    //    {
    //        um->beginNewTransaction (defaultChangeTransactionName);
    //        um->perform (new DefaultChangeAction (*this, value, currentDefault),
    //                     defaultChangeTransactionName);
    //    }
    //    else
    //    {
    //        setDefaultInternal (value);
    //    }
}

void Parameter::setDefaultInternal (float newNormalizedDefault)
{
    jassert (newNormalizedDefault >= 0.f && newNormalizedDefault <= 1.f);
    currentDefault.store (newNormalizedDefault);
    listeners.call ([&newNormalizedDefault] (Listener& l)
                    { l.parameterDefaultChanged (newNormalizedDefault); });
}

void Parameter::setDenormalizedDefault (float value)
{
    setNormalizedDefault (normalize (value));
}

void Parameter::refreshDefault()
{
    setNormalizedDefault (getNormalizedDefault());
}

void Parameter::resetToDefault()
{
    setNormalizedValue (getNormalizedDefault());
}

void Parameter::setNormalizedValue (float value)
{
    jassert (value >= 0.0f && value <= 1.0f);

    if (value == getCurrentNormalizedValue()) return;

    setValueInternal (value);

    //    if (um != nullptr)
    //    {
    //        um->perform (new ValueChangeAction (*this, value, rap.getValue()),
    //                     valueChangeTransactionName);
    //    }
    //    else
    //    {
    //        setValueInternal (value);
    //    }
}

void Parameter::setValueInternal (float newNormalizedValue)
{
    bool needToEndGesture = false;

    if (! isChanging())
    {
        beginGesture();
        needToEndGesture = true;
    }

    rap.setValueNotifyingHost (newNormalizedValue);
    listeners.call ([&newNormalizedValue] (Listener& l)
                    { l.parameterValueChanged (newNormalizedValue); });

    if (needToEndGesture)
        endGesture();
}

void Parameter::setDenormalizedValue (float value)
{
    setNormalizedValue (normalize (value));
}

float Parameter::getCurrentNormalizedValue() const
{
    return rap.getValue();
}

float Parameter::getCurrentDenormalizedValue() const
{
    return denormalize (getCurrentNormalizedValue());
}

float Parameter::normalize (float input) const
{
    return rap.convertTo0to1 (input);
}

float Parameter::denormalize (float input) const
{
    return rap.convertFrom0to1 (input);
}

void Parameter::setUndoManager (juce::UndoManager& managerToUse)
{
    um = &managerToUse;
}

void Parameter::doAction (bool force)
{
    const auto value = getCurrentNormalizedValue();

    if (force || value != lastActionedValue)
    {
        lastActionedValue = value;
        actionBroadcaster.trigger();
    }
}

void Parameter::sendListenerSyncCallback()
{
    const auto value = getCurrentNormalizedValue();
    listeners.call ([&value] (Listener& l)
                    { l.parameterValueChanged (value); });
}


void Parameter::toValueTree (juce::ValueTree& tree)
{
    tree.setProperty ("ParameterValue", getCurrentNormalizedValue(), nullptr);
    tree.setProperty ("ParameterDefaultValue", getNormalizedDefault(), nullptr);
    tree.setProperty ("MappedMidiControllerNumber", getMidiControllerNumber(), nullptr);
}

void Parameter::fromValueTree (const juce::ValueTree& tree)
{
    setNormalizedValue (tree.getProperty ("ParameterValue"));
    setNormalizedDefault (tree.getProperty ("ParameterDefaultValue"));
    setMidiControllerNumber (tree.getProperty ("MappedMidiControllerNumber"));
}


Parameter::Listener::Listener (Parameter& paramToUse)
    : param (paramToUse)
{
    param.listeners.add (this);
}

Parameter::Listener::~Listener()
{
    param.listeners.remove (this);
}

void Parameter::Listener::parameterValueChanged (float) { }

void Parameter::Listener::parameterGestureStateChanged (bool) { }

void Parameter::Listener::parameterDefaultChanged (float) { }

}  // namespace bav
