
namespace bav::plugin
{
StateBase::StateBase (String pluginName, ParameterList& paramsToUse)
    : SerializableData (pluginName + "_State"), dimensions (pluginName + "_Dimensions"), params (paramsToUse)
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
}

StateSerializer::StateSerializer (StateBase& stateToUse, StateToggler& togglerToUse)
    : state (stateToUse), toggler (togglerToUse)
{
}

void StateSerializer::serialize (TreeReflector& ref)
{
    ref.add ("State", state);
    ref.add ("Toggles", toggler);
}

}  // namespace bav::plugin
