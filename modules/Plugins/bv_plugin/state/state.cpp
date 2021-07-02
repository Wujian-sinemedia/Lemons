
namespace bav
{

PluginState::PluginState (ParameterList& listToUse, String name)
: SerializableData (name), list (listToUse)
{
    list.add (mainBypass);
}

ParameterList& PluginState::getParameters()
{
    return list;
}

void PluginState::addTo (juce::AudioProcessor& processor)
{
    list.addParametersTo (processor);
}

}
