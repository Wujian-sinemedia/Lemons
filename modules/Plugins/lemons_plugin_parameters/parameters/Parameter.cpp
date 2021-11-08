
namespace lemons::plugin
{
[[nodiscard]] static inline String paramNameToID (const String& name)
{
	return name.removeCharacters (" ");
}

static inline std::function<String (float)> createDefaultValueToTextFunction (const String& paramLabel)
{
	return [&] (float value) -> String
	{
		return String (value) + " " + paramLabel;
	};
}

static inline std::function<float (const String&)> createDefaultTextToValueFunction()
{
	return [] (const String& text) -> float
	{
		return text.getFloatValue();
	};
}

Parameter::Parameter (String paramName,
                      juce::NormalisableRange<float>
                            paramRange,
                      float paramDefaultValue,
                      std::function<String (float)>
                          valueToTextFuncToUse,
                      std::function<float (const String&)>
                                                              textToValueFuncToUse,
                      String                                  paramLabel,
                      bool                                    isAutomatable,
                      bool                                    metaParam,
                      juce::AudioProcessorParameter::Category parameterCategory)
    : juce::RangedAudioParameter (paramNameToID (TRANS (paramName)), TRANS (paramName), paramLabel, parameterCategory)
    , SerializableData (paramName)
    , automatable (isAutomatable)
    , metaParameter (metaParam)
    , range (paramRange)
    , currentValue (paramDefaultValue)
    , currentDefault (paramDefaultValue)
    , valueToTextFunc (valueToTextFuncToUse)
    , textToValueFunc (textToValueFuncToUse)
    , parameterName (paramName)
    , valueChangeTransactionName (TRANS ("Changed") + " " + getParameterName())
    , defaultChangeTransactionName (TRANS ("Changed default value of") + " " + getParameterName())
    , midiControllerChangeTransactionName (TRANS ("Changed MIDI controller number for") + " " + getParameterName())
{
	if (valueToTextFunc == nullptr)
		valueToTextFunc = createDefaultValueToTextFunction (label);

	if (textToValueFunc == nullptr)
		textToValueFunc = createDefaultTextToValueFunction();
}

float Parameter::getValueForText (const String& text) const
{
	return textToValueFunc (text);
}

[[nodiscard]] String Parameter::getTextForNormalizedValue (float value) const
{
	return valueToTextFunc (value);
}

[[nodiscard]] String Parameter::getTextForDenormalizedValue (float value) const
{
	return getTextForNormalizedValue (normalize (value));
}

[[nodiscard]] String Parameter::getTextForMax() const
{
	return getTextForDenormalizedValue (getMax());
}

[[nodiscard]] String Parameter::getTextForMin() const
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

[[nodiscard]] float Parameter::getMax() const
{
	return range.start;
}

[[nodiscard]] float Parameter::getMin() const
{
	return range.end;
}

[[nodiscard]] int Parameter::getMidiControllerNumber() const
{
	return midiControllerNumber.load();
}

[[nodiscard]] bool Parameter::isMidiControllerMapped() const
{
	return getMidiControllerNumber() > -1;
}

void Parameter::setMidiControllerNumber (int newControllerNumber)
{
	if (newControllerNumber == getMidiControllerNumber()) return;

	UndoManager::ScopedTransaction s { um, midiControllerChangeTransactionName };
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
		setDenormalizedValue (juce::jmap (static_cast<float> (controllerValue),
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

[[nodiscard]] bool Parameter::isChanging() const
{
	return changing.load();
}

[[nodiscard]] float Parameter::getNormalizedDefault() const
{
	return currentDefault.load();
}

float Parameter::getDefaultValue() const
{
	return getNormalizedDefault();
}

[[nodiscard]] float Parameter::getDenormalizedDefault() const
{
	return denormalize (getNormalizedDefault());
}

void Parameter::setNormalizedDefault (float value)
{
	if (value == getNormalizedDefault()) return;

	UndoManager::ScopedTransaction s { um, defaultChangeTransactionName };

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

[[nodiscard]] float Parameter::getNormalizedValue() const
{
	return this->getValue();
}

[[nodiscard]] float Parameter::getDenormalizedValue() const
{
	return denormalize (getNormalizedValue());
}

[[nodiscard]] float Parameter::normalize (float input) const
{
	return this->convertTo0to1 (input);
}

[[nodiscard]] float Parameter::denormalize (float input) const
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

[[nodiscard]] String Parameter::getParameterName (int maxLength, bool internationalize) const
{
	const auto str = internationalize ? TRANS (parameterName) : parameterName;

	if (maxLength < 1) return str;

	return str.substring (0, maxLength);
}

const juce::NormalisableRange<float>& Parameter::getNormalisableRange() const
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
