
namespace lemons::plugin
{
StateBase::StateBase (const String& pluginName, ParameterList& paramsToUse, SerializableData* customStateDataToUse)
    : SerializableData (pluginName + "_State"),
      modManager (paramsToUse),
      params (paramsToUse), customStateData (customStateDataToUse)
{
}

[[nodiscard]] ParameterList& StateBase::getParameters() noexcept
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

}  // namespace lemons::plugin
