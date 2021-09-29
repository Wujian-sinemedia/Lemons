
namespace lemons::plugin
{
inline String paramNameToID (const String& name)
{
    return name.removeCharacters (" ");
}

Parameter::Parameter (String paramName,
                      juce::NormalisableRange< float >
                            paramRange,
                      float paramDefaultValue,
                      std::function< String (float) >
                          valueToTextFuncToUse,
                      std::function< float (const String&) >
                                                              textToValueFuncToUse,
                      String                                  paramLabel,
                      bool                                    isAutomatable,
                      bool                                    metaParam,
                      juce::AudioProcessorParameter::Category parameterCategory)
    : juce::RangedAudioParameter (paramNameToID (TRANS (paramName)), TRANS (paramName), paramLabel, parameterCategory),
      SerializableData (paramName),
      automatable (isAutomatable),
      metaParameter (metaParam),
      range (paramRange),
      currentValue (paramDefaultValue), currentDefault (paramDefaultValue),
      valueToTextFunc (valueToTextFuncToUse),
      textToValueFunc (textToValueFuncToUse),
      parameterName (paramName),
      valueChangeTransactionName (TRANS ("Changed") + " " + getName()),
      defaultChangeTransactionName (TRANS ("Changed default value of") + " " + getName()),
      midiControllerChangeTransactionName (TRANS ("Changed MIDI controller number for") + " " + getName())
{
    if (valueToTextFunc == nullptr)
    {
        valueToTextFunc = [&] (float value)
        { return String (value) + label; };
    }

    if (textToValueFunc == nullptr)
    {
        textToValueFunc = [] (const String& text)
        { return text.getFloatValue(); };
    }
}

float Parameter::getValueForText (const String& text) const
{
    return textToValueFunc (text);
}

String Parameter::getTextForNormalizedValue (float value) const
{
    return valueToTextFunc (value);
}

String Parameter::getTextForDenormalizedValue (float value) const
{
    return getTextForNormalizedValue (normalize (value));
}

String Parameter::getTextForMax() const
{
    return getTextForDenormalizedValue (getMax());
}

String Parameter::getTextForMin() const
{
    return getTextForDenormalizedValue (getMin());
}

/* returns the normalized value */
float Parameter::getValue() const
{
    return convertTo0to1 (currentValue.load());
}

void Parameter::setValue (float newValue)
{
    currentValue.store (convertFrom0to1 (newValue));
}

float Parameter::getMax() const
{
    return range.start;
}

float Parameter::getMin() const
{
    return range.end;
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
    if (newControllerNumber == getMidiControllerNumber()) return;

    UndoManager::ScopedTransaction s {um, midiControllerChangeTransactionName};
    setMidiControllerInternal (newControllerNumber);
}

void Parameter::setMidiControllerInternal (int controller)
{
    midiControllerNumber.store (controller);
    listeners.call ([controller] (Listener& l)
                    { l.controllerNumberChanged (controller); });
}

void Parameter::removeMidiControllerMapping()
{
    midiControllerNumber.store (-1);
}

void Parameter::processNewControllerMessage (int controllerNumber, int controllerValue)
{
    if (controllerNumber == getMidiControllerNumber())
    {
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
    this->beginChangeGesture();

    listeners.call ([] (Listener& l)
                    { l.gestureStateChanged (true); });
}

void Parameter::endGesture()
{
    if (! isChanging())
        return;

    if (um != nullptr)
        um->endTransaction();

    changing.store (false);
    this->endChangeGesture();

    listeners.call ([] (Listener& l)
                    { l.gestureStateChanged (false); });
}

bool Parameter::isChanging() const
{
    return changing.load();
}

float Parameter::getNormalizedDefault() const
{
    return currentDefault.load();
}

float Parameter::getDefaultValue() const
{
    return getNormalizedDefault();
}

float Parameter::getDenormalizedDefault() const
{
    return denormalize (getNormalizedDefault());
}

void Parameter::setNormalizedDefault (float value)
{
    if (value == getNormalizedDefault()) return;

    UndoManager::ScopedTransaction s {um, defaultChangeTransactionName};

    setDefaultInternal (value);
}

void Parameter::setDefaultInternal (float value)
{
    jassert (value >= 0.0f && value <= 1.0f);

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
    if (value == getNormalizedValue()) return;

    bool needToEndGesture = false;

    if (! isChanging())
    {
        beginGesture();
        needToEndGesture = true;
    }

    setValueInternal (value);

    if (needToEndGesture)
        endGesture();
}

void Parameter::setValueInternal (float value)
{
    jassert (value >= 0.0f && value <= 1.0f);

    setValueNotifyingHost (value);
    listeners.call ([value] (Listener& l)
                    { l.parameterValueChanged (value); });
}

void Parameter::setDenormalizedValue (float value)
{
    setNormalizedValue (normalize (value));
}

float Parameter::getNormalizedValue() const
{
    return this->getValue();
}

float Parameter::getDenormalizedValue() const
{
    return denormalize (getNormalizedValue());
}

float Parameter::normalize (float input) const
{
    return this->convertTo0to1 (input);
}

float Parameter::denormalize (float input) const
{
    return this->convertFrom0to1 (input);
}

void Parameter::setUndoManager (UndoManager& managerToUse)
{
    um = &managerToUse;
}

bool Parameter::isAutomatable() const
{
    return automatable;
}

bool Parameter::isMetaParameter() const
{
    return metaParameter;
}

void Parameter::serialize (TreeReflector& ref)
{
    auto& tree = ref.getRawDataTree();

    if (ref.isLoading())
    {
        setValueInternal (tree.getProperty ("Value", getDenormalizedValue()));
        setDefaultInternal (tree.getProperty ("DefaultValue", getDenormalizedDefault()));
        setMidiControllerInternal (tree.getProperty ("MidiControllerNumber", getMidiControllerNumber()));
    }
    else
    {
        tree.setProperty ("Value", getDenormalizedValue(), nullptr);
        tree.setProperty ("DefaultValue", getDenormalizedDefault(), nullptr);
        tree.setProperty ("MidiControllerNumber", getMidiControllerNumber(), nullptr);
    }
}

String Parameter::getName (int maxLength) const
{
    const auto trans = TRANS (parameterName);

    if (maxLength < 1) return trans;

    return trans.substring (0, maxLength);
}

const juce::NormalisableRange< float >& Parameter::getNormalisableRange() const
{
    return range;
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
void Parameter::Listener::gestureStateChanged (bool) { }
void Parameter::Listener::parameterDefaultChanged (float) { }
void Parameter::Listener::controllerNumberChanged (int) { }

}  // namespace lemons::plugin
