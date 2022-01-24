namespace lemons::PluginMetadataEditor::gui
{

ProcessorAttributeComponent::ProcessorAttributeComponent (plugin::ProcessorAttributes& attributesToUse, std::function<void()> callback)
	: stateChanged (callback)
	, attributes (attributesToUse)
{
	jassert (stateChanged != nullptr);
}

}  // namespace lemons::PluginMetadataEditor::gui
