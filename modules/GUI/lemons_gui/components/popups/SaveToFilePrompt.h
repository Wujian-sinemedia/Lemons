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

#pragma once

namespace lemons::gui::components
{

using juce::ValueTree;

class SaveToFilePrompt : public PopupComponentBase
{
public:

	explicit SaveToFilePrompt (const ValueTree& dataToSave, std::function<void()> toClose);

	explicit SaveToFilePrompt (const ValueTree& dataToSave, std::unique_ptr<PopupComponentBase>& holder);

private:

	void run();

	void processFileChooserResult (const File& chosenFile);

	ValueTree data;

	juce::FileChooser chooser { "Choose a file to save to", {}, {}, true, false, this };

	// XML / JSON / opaque selector
};

}  // namespace lemons::gui::components
