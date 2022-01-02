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

ParameterList::ParameterList()
{
	add (bypass);
}

ParameterList::ParameterList (const ParameterLayout& layout)
{
	add (bypass);

	for (const auto& traits : layout.parameters)
	{
		auto parameter = traits.createParameter (*this);

		add (*(parameter.get()));
	}
}

ParameterLayout ParameterList::getParameterLayout() const
{
	ParameterLayout layout;

	for (const auto* param : params)
		layout.parameters.push_back (param->getParameterTraits());

	return layout;
}

void ParameterList::addTo (juce::AudioProcessor& processor) const
{
	std::for_each (params.begin(), params.cend(), [&] (Parameter* p)
	               { processor.addParameter (p); });
}

void ParameterList::add (Parameter& parameter)
{
	params.emplace_back (&parameter);
}

void ParameterList::processControllerMessage (int number, int value)
{
	for (auto* param : params)
		param->processNewControllerMessage (number, value);
}

bool ParameterList::isControllerMapped (int number) const
{
	for (const auto* param : params)
		if (param->getMidiControllerNumber() == number)
			return true;

	return false;
}

void ParameterList::resetAllControllerMappedParams()
{
	for (auto* param : params)
		if (param->isMidiControllerMapped())
			param->resetToDefault();
}

Parameter* ParameterList::getNamedParameter (const String& name) const
{
	for (auto* param : params)
		if (param->getParameterName() == name)
			return param;

	return nullptr;
}

juce::Array<MetaParameterBase*> ParameterList::getMetaParameters() const
{
	juce::Array<MetaParameterBase*> metaParams;

	for (auto* param : params)
	{
		if (param->isMetaParameter())
		{
			if (auto* meta = dynamic_cast<MetaParameterBase*> (param))
			{
				metaParams.add (meta);
			}
			else
			{
				jassertfalse;
			}
		}
	}

	return metaParams;
}

juce::Array<Parameter*> ParameterList::getMeterParameters() const
{
	juce::Array<Parameter*> meterParams;

	for (auto* param : params)
		if (! param->isAutomatable())
			meterParams.add (param);

	return meterParams;
}

ValueTree ParameterList::saveToValueTree() const
{
	ValueTree tree { valueTreeType };

	for (const auto* param : params)
	{
		const auto child = param->saveToValueTree();

		if (child.isValid())
			tree.appendChild (child, nullptr);
	}

	return tree;
}

void ParameterList::loadFromValueTree (const ValueTree& tree)
{
	if (! tree.hasType (valueTreeType))
		return;

	for (auto* param : params)
	{
		if (! param->isAutomatable())
			continue;

		param->loadFromValueTree (tree.getChildWithProperty (Parameter::id_prop, param->getParameterID()));
		param->refreshDefault();
	}
}


ParameterList::Listener::Listener (const ParameterList& list,
                                   std::function<void (Parameter&)>
                                       onParamChange,
                                   std::function<void (Parameter&, bool)>
                                       onGestureGhange)
{
	for (auto* param : list.params)
	{
		const auto change = [=]
		{
			if (onParamChange)
				onParamChange (*param);
		};

		const auto gesture = [=] (bool starting)
		{
			if (onGestureGhange)
				onGestureGhange (*param, starting);
		};

		updaters.add (new ParamUpdater (*param, std::move (change), std::move (gesture)));
	}
}

}  // namespace lemons::plugin
