
namespace bav::plugin
{
void Dimensions::serialize (TreeReflector& ref)
{
    ref.add ("Width", width);
    ref.add ("Height", height);
}


State::State (String pluginName)
    : SerializableData (pluginName + "_State"), dimensions (pluginName + "_Dimensions")
{
    // list.add (mainBypass);
}

void State::setDimensions (int width, int height)
{
    dimensions.width  = width;
    dimensions.height = height;
}

void State::addTo (juce::AudioProcessor& processor)
{
    getParameters().addParametersTo (processor);
}

void State::serialize (TreeReflector& ref)
{
    ref.add ("EditorDimensions", dimensions);
    ref.add ("Parameters", getParameters());
}

}  // namespace bav::plugin
