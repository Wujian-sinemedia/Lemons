namespace lemons::PluginMetadataEditor::gui
{

EditorAttributeComponent::EditorAttributeComponent (plugin::EditorAttributes& attributesToUse, std::function<void()> callback)
	: stateChanged (callback), attributes (attributesToUse)
{
	jassert (stateChanged != nullptr);
}

}  // namespace lemons::PluginMetadataEditor::gui
