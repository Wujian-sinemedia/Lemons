namespace lemons::gui::components
{

juce::AudioPluginFormatManager& getPluginFormatManager()
{
    struct DefaultPluginFormats final : public juce::AudioPluginFormatManager
    {
        explicit DefaultPluginFormats()
        {
            addDefaultFormats();
        }
    };
    
    static DefaultPluginFormats manager;
    
    return manager;
}


/*-------------------------------------------------------------------------------------------------------------------------*/


PluginDetailView::PluginDetailView (const juce::PluginDescription& descriptionToUse)
: description(descriptionToUse)
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
    return getPluginFormatManager().createPluginInstance (description, initialSampleRate, initialBlocksize, error);
}


/*-------------------------------------------------------------------------------------------------------------------------*/

PluginCategoryComponent::PluginCategoryComponent (const PluginCategory& category)
{
    for (const auto* subcat : category.getSubcategories())
        subcategories.add (new PluginCategoryComponent (*subcat));
    
    for (const auto& plugin : category.getPlugins())
        plugins.add (new PluginDetailView (plugin));
}

}
