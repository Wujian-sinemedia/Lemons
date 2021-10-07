
namespace lemons::plugin
{
ParamHolderBase::ParamHolderBase (bool internal)
    : isInternal (internal)
{
}

Parameter* ParamHolderBase::operator->()
{
    return getParam();
}

void ParamHolderBase::addTo (juce::AudioProcessor& processor)
{
    processor.addParameter (getParam());
    addedToProcessor = true;
}

void ParamHolderBase::serialize (TreeReflector& ref)
{
    ref.as (*getParam());
}

}  // namespace lemons::plugin
