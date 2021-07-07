
namespace bav
{

void PluginDimensions::serialize (TreeReflector& ref)
{
    ref.add ("Width", width);
    ref.add ("Height", height);
}


PluginState::PluginState (ParameterList& listToUse, String pluginName)
: SerializableData (pluginName + "_State"), dimensions(pluginName + "_Dimensions"), list (listToUse)
{
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

void PluginState::serialize (TreeReflector& ref)
{
    ref.add ("EditorDimensions", dimensions);
    ref.add ("Parameters", getParameters());
}

}
