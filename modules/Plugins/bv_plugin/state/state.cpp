
namespace bav::plugin
{
void Dimensions::serialize (TreeReflector& ref)
{
    ref.add ("Width", width);
    ref.add ("Height", height);
}

Dimensions& Dimensions::operator= (const juce::Point< int >& newSize) noexcept
{
    width  = newSize.x;
    height = newSize.y;

    return *this;
}

juce::Point< int > Dimensions::get() const
{
    return {width, height};
}


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
}

void StateBase::setUndoManager (UndoManager& undo)
{
    params.setUndoManager (undo);
}

}  // namespace bav::plugin
