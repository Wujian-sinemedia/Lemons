
namespace lemons::plugin
{
ParamHolderBase::ParamHolderBase (bool internal) noexcept
    : isInternal (internal)
{
}

Parameter* ParamHolderBase::operator->() const noexcept
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
