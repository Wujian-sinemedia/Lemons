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
		auto parameter = createParameter (traits);

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

ValueTree ParameterList::saveToValueTree() const
{
	ValueTree tree { valueTreeType };

	for (const auto* param : params)
		tree.appendChild (param->saveToValueTree(), nullptr);

	return tree;
}

void ParameterList::loadFromValueTree (const ValueTree& tree)
{
	if (! tree.hasType (valueTreeType))
		return;

	for (auto* param : params)
	{
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
