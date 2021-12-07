
namespace lemons::plugin
{
StateBase::StateBase (const String& pluginName)
{
}

void StateBase::addTo (juce::AudioProcessor& processor)
{
	//	params.add (mainBypass);
	//	params.addParametersTo (processor);
}

void StateBase::addAllAsInternal()
{
	//	params.add (mainBypass);
	//	params.addAllParametersAsInternal();
}

}  // namespace lemons::plugin
