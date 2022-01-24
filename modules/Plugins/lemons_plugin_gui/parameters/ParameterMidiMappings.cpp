namespace lemons::gui::components
{

ParameterMidiMappingEditor::ParameterMidiMappingEditor (plugin::Parameter& parameterToUse)
	: parameter (parameterToUse)
{
}


ParameterListMIDIMappingsEditor::ParameterListMIDIMappingsEditor (plugin::ParameterList& listToUse)
	: list (listToUse)
{
	for (auto* param : list.getAllParameters())
		parameterEditors.createAndAddTo (*this, *param);
}

}  // namespace lemons::gui::components
