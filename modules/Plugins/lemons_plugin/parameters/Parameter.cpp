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
	const ParameterRange&			paramRange,
	float							paramDefaultValue,
	BasicValToStringFunc			valueToTextFuncToUse,
	BasicStringToValFunc			textToValueFuncToUse,
	const String&					paramLabel,
	bool							isAutomatable,
	bool							metaParam,
	ParameterCategory				parameterCategory)
	: juce::RangedAudioParameter (detail::paramNameToID (TRANS (paramName)), TRANS (paramName), paramLabel, parameterCategory), automatable (isAutomatable), metaParameter (metaParam), range (paramRange), currentValue (paramDefaultValue), currentDefault (paramDefaultValue), valueToTextFunc (valueToTextFuncToUse), textToValueFunc (textToValueFuncToUse)
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

	traits.name			   = getName (50);
	traits.range		   = range;
	traits.defaultValue	   = currentDefault.load();
	traits.valueToText	   = valueToTextFunc;
	traits.textToValue	   = textToValueFunc;
	traits.label		   = getLabel();
	traits.isAutomatable   = automatable;
	traits.isMetaParameter = metaParameter;
	traits.category		   = getCategory();

	return traits;
}


void Parameter::setMidiControllerNumber (int newControllerNumber)
{
	if (newControllerNumber == midiControllerNumber.load()) return;

	midiControllerNumber.store (newControllerNumber);

	listeners.call ([newControllerNumber] (Listener& l)
		{ l.controllerNumberChanged (newControllerNumber); });
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

	changing.store (true);
	this->beginChangeGesture();

	listeners.call ([] (Listener& l)
		{ l.gestureStateChanged (true); });
}

void Parameter::endGesture()
{
	if (! isChanging())
		return;

	changing.store (false);
	this->endChangeGesture();

	listeners.call ([] (Listener& l)
		{ l.gestureStateChanged (false); });
}


void Parameter::setNormalizedDefault (float value)
{
	if (value == getNormalizedDefault()) return;

	jassert (value >= 0.0f && value <= 1.0f);

	currentDefault.store (value);
	listeners.call ([value] (Listener& l)
		{ l.parameterDefaultChanged (value); });
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

	jassert (value >= 0.0f && value <= 1.0f);

	setValueNotifyingHost (value);
	listeners.call ([value] (Listener& l)
		{ l.parameterValueChanged (value); });

	if (needToEndGesture)
		endGesture();
}


float Parameter::normalize (float input) const noexcept
{
	return this->convertTo0to1 (input);
}

float Parameter::denormalize (float input) const noexcept
{
	return this->convertFrom0to1 (input);
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


/*---------------------------------------------------------------------------------------------------------------------------*/


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

/*---------------------------------------------------------------------------------------------------------------------------*/

Parameter::LambdaListener::LambdaListener (Parameter& parameter,
	std::function<void (float)>
		valueChanged,
	std::function<void (float)>
		defaultChanged,
	std::function<void (bool)>
		gestureChanged,
	std::function<void (int)>
		midiControllerChanged)
	: Listener (parameter), valueChangeFunc (valueChanged), defaultChangeFunc (defaultChanged), gestureChangeFunc (gestureChanged), controllerChangeFunc (midiControllerChanged)
{
}

void Parameter::LambdaListener::parameterValueChanged (float newNormalizedValue)
{
	if (valueChangeFunc != nullptr)
		valueChangeFunc (newNormalizedValue);
}

void Parameter::LambdaListener::gestureStateChanged (bool gestureIsStarting)
{
	if (gestureChangeFunc != nullptr)
		gestureChangeFunc (gestureIsStarting);
}

void Parameter::LambdaListener::parameterDefaultChanged (float newNormalizedDefault)
{
	if (defaultChangeFunc != nullptr)
		defaultChangeFunc (newNormalizedDefault);
}

void Parameter::LambdaListener::controllerNumberChanged (int newControllerNumber)
{
	if (controllerChangeFunc != nullptr)
		controllerChangeFunc (newControllerNumber);
}

}  // namespace lemons::plugin
