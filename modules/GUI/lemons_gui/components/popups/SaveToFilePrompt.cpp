namespace lemons::gui::components
{

SaveToFilePrompt::SaveToFilePrompt (const ValueTree& dataToSave, std::function<void()> toClose)
    : PopupComponentBase (toClose)
    , data (dataToSave)
{
	run();
}

SaveToFilePrompt::SaveToFilePrompt (const ValueTree& dataToSave, std::unique_ptr<PopupComponentBase>& holder)
    : PopupComponentBase (holder)
    , data (dataToSave)
{
	run();
}

void SaveToFilePrompt::run()
{
	chooser.launchAsync (juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::warnAboutOverwriting | juce::FileBrowserComponent::saveMode,
	                     [&] (const juce::FileChooser& f)
	                     { this->processFileChooserResult (f.getResult()); });
}

void SaveToFilePrompt::processFileChooserResult (const File& chosenFile)
{
	bool asXML = false, asJSON = true;

	if (asXML)
	{
		files::saveValueTree<files::FileType::XML> (chosenFile, data);
	}
	else if (asJSON)
	{
		files::saveValueTree<files::FileType::JSON> (chosenFile, data);
	}
	else
	{
		files::saveValueTree<files::FileType::Opaque> (chosenFile, data);
	}

	close();
}

}  // namespace lemons::gui::components
