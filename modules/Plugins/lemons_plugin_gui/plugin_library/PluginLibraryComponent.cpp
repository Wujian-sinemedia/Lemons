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
 *  ======================================================================================
 */

namespace lemons::gui::components
{


PluginDetailView::PluginDetailView (const juce::PluginDescription& descriptionToUse)
	: description (descriptionToUse)
{
	util::addAndMakeVisible (*this, name, descriptiveName, version, numInChannels, numOutChannels, category, manufacturer);
}

void PluginDetailView::resized()
{
	// to do
}

std::unique_ptr<juce::AudioPluginInstance> PluginDetailView::createPluginInstance (double initialSampleRate, int initialBlocksize) const
{
	String error;

	return plugin::scanning::getDefaultPluginFormatManager().createPluginInstance (description, initialSampleRate, initialBlocksize, error);
}


/*-------------------------------------------------------------------------------------------------------------------------*/

PluginCategoryComponent::PluginCategoryComponent (const plugin::scanning::Category& category)
{
	for (const auto* subcat : category.getSubcategories())
	{
		auto* subcategory = subcategories.add (new PluginCategoryComponent (*subcat));
		addAndMakeVisible (subcategory);
	}

	for (const auto& description : category.getPlugins())
		plugins.createAndAddTo (*this, description);

	categoryName.setText (category.getName(),
						  juce::NotificationType::dontSendNotification);

	addAndMakeVisible (categoryName);
}


/*-------------------------------------------------------------------------------------------------------------------------*/

PluginLibraryComponent::PluginLibraryComponent (juce::FileSearchPath searchPath,
												const File&			 blacklistFile,
												SortMethod			 initialSortMethod)
	: rootPath (searchPath), blacklist (blacklistFile)
{
	addAndMakeVisible (rootCategoryComponent);

	resort (initialSortMethod);
}

void PluginLibraryComponent::resort (SortMethod sortMethod)
{
	const auto rootCategory = plugin::scanning::scanDirectory (rootPath, blacklist, sortMethod);

	rootCategoryComponent.create (*rootCategory);
}

}  // namespace lemons::gui::components
