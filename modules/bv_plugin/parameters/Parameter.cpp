
namespace bav
{
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

void Parameter::processNewControllerMessage (int controllerNumber, int controllerValue)
{
    if (controllerNumber == getMidiControllerNumber())
    {
        const auto& range = rap.getNormalisableRange();
        setDenormalizedValue (juce::jmap (static_cast< float > (controllerValue),
                                          0.f, 127.f,
                                          range.start, range.end));
    }
}

void Parameter::beginGesture()
{
    if (isChanging())
        return;

    if (um != nullptr)
        um->beginNewTransaction (valueChangeTransactionName);

    changing.store (true);
    rap.beginChangeGesture();

    listeners.call ([] (Listener& l)
                    { l.parameterGestureStateChanged (true); });
}

void Parameter::endGesture()
{
    if (! isChanging())
        return;

    if (um != nullptr)
        um->endTransaction();

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

    UndoManager::ScopedTransaction s {um, defaultChangeTransactionName};

    currentDefault.store (value);
    listeners.call ([value] (Listener& l)
                    { l.parameterDefaultChanged (value); });
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

    bool needToEndGesture = false;

    if (! isChanging())
    {
        beginGesture();
        needToEndGesture = true;
    }

    rap.setValueNotifyingHost (value);
    listeners.call ([value] (Listener& l)
                    { l.parameterValueChanged (value); });

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

void Parameter::setUndoManager (UndoManager& managerToUse)
{
    um = &managerToUse;
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
