
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

namespace lemons::plugin
{

ParameterList::ParameterList()
{
	createAndAddBypassParameter();
}

ParameterList::ParameterList (const ParameterLayout& layout)
{
	createAndAddBypassParameter();

	for (const auto& traits : layout.parameters)
		add (traits.createParameter (*this));
}

Parameter& ParameterList::add (std::unique_ptr<Parameter> parameter)
{
	return parameters.add (new Holder (std::move (parameter)))->getParameter();
}

void ParameterList::createAndAddBypassParameter()
{
	bypass = dynamic_cast<ToggleParameter*> (&add (std::make_unique<ToggleParameter> ("Bypass", false)));

	jassert (bypass != nullptr);
}

ToggleParameter& ParameterList::getBypass() const
{
	jassert (bypass != nullptr);
	return *bypass;
}

ParameterLayout ParameterList::getParameterLayout() const
{
	ParameterLayout layout;

	for (const auto* param : parameters)
		layout.parameters.push_back (param->getParameter().getParameterTraits());

	return layout;
}

void ParameterList::addTo (juce::AudioProcessor& processor) const
{
	for (auto* param : parameters)
		param->addToProcessor (processor);
}

void ParameterList::processControllerMessage (int number, int value)
{
	for (const auto* param : parameters)
		param->getParameter().processNewControllerMessage (number, value);
}

bool ParameterList::isControllerMapped (int number) const
{
	for (const auto* param : parameters)
		if (param->getParameter().getMidiControllerNumber() == number)
			return true;

	return false;
}

void ParameterList::resetAllControllerMappedParams()
{
	for (const auto* param : parameters)
	{
		auto& parameter = param->getParameter();

		if (parameter.isMidiControllerMapped())
			parameter.resetToDefault();
	}
}

Parameter* ParameterList::getParameterWithName (const String& name) const
{
	for (const auto* param : parameters)
	{
		auto& parameter = param->getParameter();

		if (parameter.getName (50) == name)
			return &parameter;
	}

	return nullptr;
}

Parameter* ParameterList::getParameterWithID (const String& paramID) const
{
	for (const auto* param : parameters)
	{
		auto& parameter = param->getParameter();

		if (parameter.getParameterID() == paramID)
			return &parameter;
	}

	return nullptr;
}

juce::Array<MetaParameterBase*> ParameterList::getMetaParameters() const
{
	juce::Array<MetaParameterBase*> metaParams;

	for (const auto* param : parameters)
	{
		auto& parameter = param->getParameter();

		if (! parameter.isMetaParameter())
			continue;

		if (auto* meta = dynamic_cast<MetaParameterBase*> (&parameter))
		{
			metaParams.add (meta);
		}
		else
		{
			jassertfalse;
		}
	}

	return metaParams;
}

juce::Array<Parameter*> ParameterList::getMeterParameters() const
{
	juce::Array<Parameter*> meterParams;

	for (const auto* param : parameters)
	{
		auto& parameter = param->getParameter();

		if (! parameter.isAutomatable())
			meterParams.add (&parameter);
	}

	return meterParams;
}

juce::Array<Parameter*> ParameterList::getAllParameters() const
{
	juce::Array<Parameter*> params;

	for (const auto* param : parameters)
		params.add (&param->getParameter());

	return params;
}

ValueTree ParameterList::saveToValueTree() const
{
	ValueTree tree { valueTreeType };

	for (const auto* param : parameters)
	{
		const auto child = param->getParameter().saveToValueTree();

		if (child.isValid())
			tree.appendChild (child, nullptr);
	}

	return tree;
}

void ParameterList::loadFromValueTree (const ValueTree& tree)
{
	if (! tree.hasType (valueTreeType))
		return;

	for (const auto* param : parameters)
	{
		auto& parameter = param->getParameter();

		if (! parameter.isAutomatable())
			continue;

		parameter.loadFromValueTree (tree.getChildWithProperty (Parameter::id_prop, parameter.getParameterID()));
		parameter.refreshDefault();
	}
}

/*-----------------------------------------------------------------------------------------------------------------------*/

ParameterList::Holder::Holder (std::unique_ptr<Parameter> parameter)
	: param (parameter.release())
{
	jassert (param != nullptr);
}

ParameterList::Holder::~Holder()
{
	if (! addedToProcessor)
	{
		jassert (param != nullptr);
		delete param;
	}
}

void ParameterList::Holder::addToProcessor (juce::AudioProcessor& processor)
{
	jassert (! addedToProcessor);

	processor.addParameter (param);

	addedToProcessor = true;
}

Parameter& ParameterList::Holder::getParameter() const
{
	jassert (param != nullptr);
	return *param;
}

/*-----------------------------------------------------------------------------------------------------------------------*/

ParameterList::Listener::Listener (const ParameterList& list,
								   std::function<void (Parameter&)>
									   onParamValueChange,
								   std::function<void (Parameter&)>
									   onParamDefaultChange,
								   std::function<void (Parameter&, bool)>
									   onParamGestureGhange,
								   std::function<void (Parameter&)>
									   onParamControllerChange)
{
	for (const auto* param : list.parameters)
	{
		auto* parameter = &param->getParameter();

		auto change = [parameter, onParamValueChange] (float)  // NOLINT
		{
			if (onParamValueChange)
				onParamValueChange (*parameter);
		};

		auto default_ = [parameter, onParamDefaultChange] (float)  // NOLINT
		{
			if (onParamDefaultChange)
				onParamDefaultChange (*parameter);
		};

		auto gesture = [parameter, onParamGestureGhange] (bool starting)
		{
			if (onParamGestureGhange)
				onParamGestureGhange (*parameter, starting);
		};

		auto controller = [parameter, onParamControllerChange] (int)  // NOLINT
		{
			if (onParamControllerChange)
				onParamControllerChange (*parameter);
		};

		updaters.add (new Parameter::LambdaListener (*parameter, std::move (change), std::move (default_), std::move (gesture), std::move (controller)));
	}
}

}  // namespace lemons::plugin
