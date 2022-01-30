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

namespace lemons::gui::components
{

SaveToFilePrompt::SaveToFilePrompt (const ValueTree& dataToSave, std::function<void()> toClose)
	: PopupComponentBase (toClose), data (dataToSave)
{
	run();
}

SaveToFilePrompt::SaveToFilePrompt (const ValueTree& dataToSave, std::unique_ptr<PopupComponentBase>& holder)
	: PopupComponentBase (holder), data (dataToSave)
{
	run();
}

void SaveToFilePrompt::run()
{
	chooser.launchAsync (juce::FileBrowserComponent::canSelectFiles | juce::FileBrowserComponent::warnAboutOverwriting | juce::FileBrowserComponent::saveMode,
						 [this] (const juce::FileChooser& f)
						 { this->processFileChooserResult (f.getResult()); });
}

void SaveToFilePrompt::processFileChooserResult (const File& chosenFile)
{
	// TO DO: enable choosing JSON, XML, or opaque

	files::saveValueTree<files::FileType::JSON> (chosenFile, data);

	close();
}

}  // namespace lemons::gui::components
