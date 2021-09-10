
namespace bav::plugin
{
StateBase::StateBase (String pluginName, ParameterList& paramsToUse, SerializableData* customStateDataToUse)
    : SerializableData (pluginName + "_State"), dimensions (pluginName + "_Dimensions"), params (paramsToUse), customStateData (customStateDataToUse)
{
}

ParameterList& StateBase::getParameters()
{
    return params;
}

void StateBase::addTo (juce::AudioProcessor& processor)
{
    params.add (mainBypass);
    params.addParametersTo (processor);
}

void StateBase::addAllAsInternal()
{
    params.add (mainBypass);
    params.addAllParametersAsInternal();
}

void StateBase::serialize (TreeReflector& ref)
{
    ref.add ("EditorDimensions", dimensions);
    ref.add ("Parameters", params);
    ref.add ("ModMatrix", modManager);

    if (customStateData != nullptr)
        ref.add ("StateData", *customStateData);
}

}  // namespace bav::plugin
