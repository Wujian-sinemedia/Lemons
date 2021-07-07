
namespace bav
{

void PluginDimensions::toValueTree (ValueTree& tree)
{
    tree.setProperty ("Width", width, nullptr);
    tree.setProperty ("Height", height, nullptr);
}

void PluginDimensions::fromValueTree (const ValueTree& tree)
{
    width  = tree.getProperty ("Width");
    height = tree.getProperty ("Height");
}

PluginState::PluginState (ParameterList& listToUse, String pluginName)
: SerializableData (pluginName + "_State"), dimensions(pluginName + "_Dimensions"), list (listToUse)
{
    addDataChild (dimensions);
}

ParameterList& PluginState::getParameters()
{
    return list;
}

void PluginState::setDimensions (int width, int height)
{
    dimensions.width  = width;
    dimensions.height = height;
}

void PluginState::addTo (juce::AudioProcessor& processor)
{
    list.add (mainBypass);
    list.addParametersTo (processor);
}

}
