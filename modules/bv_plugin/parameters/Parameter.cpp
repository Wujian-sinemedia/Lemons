
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
                      juce::String paramNameShort,
                      juce::String paramNameVerbose,
                      bool automatable,
                      bool metaParam)
    : SerializableData (paramNameVerbose),
      rap (p),
      parameterNameShort (TRANS (paramNameShort)),
      parameterNameVerbose (TRANS (paramNameVerbose)),
      isAutomatable (automatable),
      isMetaParameter (metaParam),
      valueChangeTransactionName (TRANS ("Changed") + " " + parameterNameVerbose),
      defaultChangeTransactionName (TRANS ("Changed default value of") + " " + parameterNameVerbose)
{
    currentDefault    = rap.getDefaultValue();
    lastActionedValue = currentDefault;
}

bool Parameter::operator== (const Parameter& other)
{
    return SerializableData::dataIdentifier == other.dataIdentifier;
}

void Parameter::beginGesture()
{
    if (changing)
        return;

    if (um != nullptr)
    {
        um->beginNewTransaction (valueChangeTransactionName);
    }

    changing = true;
    rap.beginChangeGesture();

    listeners.call ([] (Listener& l)
                    { l.parameterGestureStateChanged (true); });
}

void Parameter::endGesture()
{
    if (! changing)
        return;

    changing = false;
    rap.endChangeGesture();

    listeners.call ([] (Listener& l)
                    { l.parameterGestureStateChanged (false); });
}

bool Parameter::isChanging() const
{
    return changing;
}

float Parameter::getNormalizedDefault() const
{
    return currentDefault;
}

float Parameter::getDenormalizedDefault() const
{
    return denormalize (currentDefault);
}

void Parameter::setNormalizedDefault (float value)
{
    jassert (value >= 0.0f && value <= 1.0f);

    if (currentDefault == value) return;

    if (um != nullptr)
    {
        um->beginNewTransaction (defaultChangeTransactionName);
        um->perform (new DefaultChangeAction (*this, value, currentDefault),
                     defaultChangeTransactionName);
    }
    else
    {
        setDefaultInternal (value);
    }
}

void Parameter::setDefaultInternal (float newNormalizedDefault)
{
    currentDefault = newNormalizedDefault;
    listeners.call ([&newNormalizedDefault] (Listener& l)
                    { l.parameterDefaultChanged (newNormalizedDefault); });
}

void Parameter::setDenormalizedDefault (float value)
{
    setNormalizedDefault (normalize (value));
}

void Parameter::refreshDefault()
{
    currentDefault = getCurrentNormalizedValue();
}

void Parameter::resetToDefault()
{
    rap.setValueNotifyingHost (currentDefault);
}

void Parameter::setNormalizedValue (float value)
{
    jassert (value >= 0.0f && value <= 1.0f);

    if (value == rap.getValue()) return;

    if (um != nullptr)
    {
        um->perform (new ValueChangeAction (*this, value, rap.getValue()),
                     valueChangeTransactionName);
    }
    else
    {
        setValueInternal (value);
    }
}

void Parameter::setValueInternal (float newNormalizedValue)
{
    bool needToEndGesture = false;

    if (! changing)
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
    return rap.convertFrom0to1 (rap.getValue());
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
