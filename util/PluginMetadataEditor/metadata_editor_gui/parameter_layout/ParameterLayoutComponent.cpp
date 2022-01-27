namespace lemons::PluginMetadataEditor::gui
{

ParameterMetadataComponent::ParameterMetadataComponent (plugin::ParameterTraits& traitsToUse, std::function<void()> callback)
	: stateChanged (callback), traits (traitsToUse)
{
	jassert (stateChanged != nullptr);
}


ParameterLayoutComponent::ParameterLayoutComponent (plugin::ParameterLayout& layoutToUse, std::function<void()> callback)
	: stateChanged (callback), layout (layoutToUse)
{
	jassert (stateChanged != nullptr);

	for (auto& traits : layout.parameters)
		parameterComponents.add (new ParameterMetadataComponent (traits, [this]()
																 { this->stateChanged(); }));
}

ParameterMetadataComponent& ParameterLayoutComponent::addNewParameter()
{
	auto& newComp = *parameterComponents.add (new ParameterMetadataComponent (layout.parameters.emplace_back(),
																			  [this]()
																			  { this->stateChanged(); }));

	stateChanged();

	return newComp;
}

void ParameterLayoutComponent::removeParameter (plugin::ParameterTraits& traits)
{
	for (auto* child : parameterComponents)
	{
		if (&child->getTraits() == &traits)
		{
			parameterComponents.removeObject (child);
			stateChanged();
			return;
		}
	}
}

}  // namespace lemons::PluginMetadataEditor::gui
