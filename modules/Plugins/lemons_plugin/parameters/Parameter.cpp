/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

namespace lemons::plugin
{


Parameter::Parameter (const String& paramName,
                      juce::NormalisableRange<float>
                                           paramRange,
                      float                paramDefaultValue,
                      BasicValToStringFunc valueToTextFuncToUse,
                      BasicStringToValFunc textToValueFuncToUse,
                      const String&        paramLabel,
                      bool                 isAutomatable,
                      bool                 metaParam,
                      ParameterCategory    parameterCategory)
    : juce::RangedAudioParameter (detail::paramNameToID (TRANS (paramName)), TRANS (paramName), paramLabel, parameterCategory)
    , automatable (isAutomatable)
    , metaParameter (metaParam)
    , range (paramRange)
    , currentValue (paramDefaultValue)
    , currentDefault (paramDefaultValue)
    , valueToTextFunc (valueToTextFuncToUse)
    , textToValueFunc (textToValueFuncToUse)
    , valueChangeTransactionName (TRANS ("Changed") + " " + getParameterName())
    , defaultChangeTransactionName (TRANS ("Changed default value of") + " " + getParameterName())
    , midiControllerChangeTransactionName (TRANS ("Changed MIDI controller number for") + " " + getParameterName())
{
	if (valueToTextFunc == nullptr)
		valueToTextFunc = detail::createDefaultValueToTextFunction (label);

	if (textToValueFunc == nullptr)
		textToValueFunc = detail::createDefaultTextToValueFunction();
}

Parameter::Parameter (const ParameterTraits& traits)
    : Parameter (traits.name, traits.range, traits.defaultValue, traits.valueToText, traits.textToValue, traits.label, traits.isAutomatable, traits.isMetaParameter, traits.category)
{
}

ParameterTraits Parameter::getParameterTraits() const
{
	ParameterTraits traits;

	traits.name            = getName (50);
	traits.range           = range;
	traits.defaultValue    = currentDefault.load();
	traits.valueToText     = valueToTextFunc;
	traits.textToValue     = textToValueFunc;
	traits.label           = getLabel();
	traits.isAutomatable   = automatable;
	traits.isMetaParameter = metaParameter;
	traits.category        = getCategory();

	return traits;
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
	return getTextForDenormalizedValue (range.end);
}

String Parameter::getTextForMin() const
{
	return getTextForDenormalizedValue (range.start);
}

float Parameter::getValue() const
{
	return currentValue.load();
}

void Parameter::setValue (float newValue)
{
	currentValue.store (newValue);
}

int Parameter::getMidiControllerNumber() const noexcept
{
	return midiControllerNumber.load();
}

bool Parameter::isMidiControllerMapped() const noexcept
{
	return getMidiControllerNumber() > -1;
}

void Parameter::setMidiControllerNumber (int newControllerNumber)
{
	if (newControllerNumber == getMidiControllerNumber()) return;

	//	UndoManager::ScopedTransaction s { um, midiControllerChangeTransactionName };

	midiControllerNumber.store (newControllerNumber);

	listeners.call ([=] (Listener& l)
	                { l.controllerNumberChanged (newControllerNumber); });
}

void Parameter::removeMidiControllerMapping() noexcept
{
	midiControllerNumber.store (-1);
}

bool Parameter::processNewControllerMessage (int controllerNumber, int controllerValue)
{
	jassert (controllerNumber >= 0);
	jassert (controllerValue >= 0 && controllerValue <= 127);

	if (controllerNumber == midiControllerNumber.load())
	{
		setDenormalizedValue (juce::jmap (static_cast<float> (controllerValue),
		                                  0.f, 127.f,
		                                  range.start, range.end));

		return true;
	}

	return false;
}

void Parameter::beginGesture()
{
	if (isChanging())
		return;

	//	if (um != nullptr)
	//		um->beginNewTransaction (valueChangeTransactionName);

	changing.store (true);
	this->beginChangeGesture();

	listeners.call ([] (Listener& l)
	                { l.gestureStateChanged (true); });
}

void Parameter::endGesture()
{
	if (! isChanging())
		return;

	//	if (um != nullptr)
	//		um->endTransaction();

	changing.store (false);
	this->endChangeGesture();

	listeners.call ([] (Listener& l)
	                { l.gestureStateChanged (false); });
}

bool Parameter::isChanging() const noexcept
{
	return changing.load();
}

float Parameter::getNormalizedDefault() const noexcept
{
	return currentDefault.load();
}

float Parameter::getDefaultValue() const
{
	return getNormalizedDefault();
}

float Parameter::getDenormalizedDefault() const noexcept
{
	return denormalize (getNormalizedDefault());
}

void Parameter::setNormalizedDefault (float value)
{
	if (value == getNormalizedDefault()) return;

	//	UndoManager::ScopedTransaction s { um, defaultChangeTransactionName };

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

float Parameter::getNormalizedValue() const noexcept
{
	return currentValue.load();
}

float Parameter::getDenormalizedValue() const noexcept
{
	return denormalize (currentValue.load());
}

float Parameter::normalize (float input) const noexcept
{
	return this->convertTo0to1 (input);
}

float Parameter::denormalize (float input) const noexcept
{
	return this->convertFrom0to1 (input);
}

// void Parameter::setUndoManager (UndoManager& managerToUse)
//{
//	um = &managerToUse;
// }

bool Parameter::isAutomatable() const
{
	return automatable;
}

bool Parameter::isMetaParameter() const
{
	return metaParameter;
}

String Parameter::getParameterName (int maxLength, bool internationalize) const
{
	const auto str = internationalize ? TRANS (name) : name;

	if (maxLength < 1) return str;

	return str.substring (0, maxLength);
}

const juce::NormalisableRange<float>& Parameter::getNormalisableRange() const
{
	return range;
}

ValueTree Parameter::saveToValueTree() const
{
	ValueTree tree { valueTreeType };

	tree.setProperty (id_prop, getParameterID(), nullptr);

	tree.setProperty (value_prop, getDenormalizedValue(), nullptr);
	tree.setProperty (default_prop, getDenormalizedDefault(), nullptr);
	tree.setProperty (controller_prop, midiControllerNumber.load(), nullptr);

	return tree;
}

void Parameter::loadFromValueTree (const ValueTree& tree)
{
	if (! tree.hasType (valueTreeType))
		return;

	if (tree.hasProperty (value_prop))
		setDenormalizedValue ((float) tree.getProperty (value_prop));

	if (tree.hasProperty (default_prop))
		setDenormalizedDefault ((float) tree.getProperty (default_prop));

	if (tree.hasProperty (controller_prop))
		midiControllerNumber.store ((int) tree.getProperty (controller_prop));
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
