/*
 ======================================================================================

 ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝

 This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.

 ======================================================================================
 */

namespace lemons::plugin
{

ProcessorHolder::ProcessorHolder (juce::AudioProcessor& processorToUse)
	: processor (processorToUse)
{
}

juce::AudioProcessor* ProcessorHolder::operator->()
{
	return &processor;
}

const juce::AudioProcessor* ProcessorHolder::operator->() const
{
	return &processor;
}

template <typename SampleType>
bool ProcessorHolder::prepareForPlayback (double samplerate, int blocksize, int numChannels)
{
	jassert (samplerate > 0. && blocksize > 0);

	if constexpr (std::is_same_v<SampleType, float>)
		processor.setProcessingPrecision (juce::AudioProcessor::ProcessingPrecision::singlePrecision);
	else
		processor.setProcessingPrecision (juce::AudioProcessor::ProcessingPrecision::doublePrecision);

	processor.setRateAndBufferSizeDetails (samplerate, blocksize);
	processor.prepareToPlay (samplerate, blocksize);

	juce::AudioProcessor::BusesLayout layout;

	const auto channelSet = juce::AudioChannelSet::canonicalChannelSet (numChannels);

	layout.inputBuses.add (channelSet);
	layout.outputBuses.add (channelSet);

	if (! processor.setBusesLayout (layout))
		return false;

	processor.enableAllBuses();

	return true;
}

template bool ProcessorHolder::prepareForPlayback<float> (double, int, int);
template bool ProcessorHolder::prepareForPlayback<double> (double, int, int);

juce::AudioProcessorParameter* ProcessorHolder::getNamedParameter (const String& name)
{
	for (auto* parameter : processor.getParameters())
		if (parameter->getName (50) == name)
			return parameter;

	return nullptr;
}

const juce::AudioProcessorParameter* ProcessorHolder::getNamedParameter (const String& name) const
{
	for (const auto* parameter : processor.getParameters())
		if (parameter->getName (50) == name)
			return parameter;

	return nullptr;
}

ProcessorHolder::ParameterState ProcessorHolder::getStateOfParameters() const
{
	ParameterState state;

	for (const auto* parameter : processor.getParameters())
	{
		ParameterStateData data;
		data.name  = parameter->getName (50);
		data.value = parameter->getValue();
		state.add (data);
	}

	return state;
}

void ProcessorHolder::setStateOfParameters (const ParameterState& state)
{
	for (const auto& paramState : state)
	{
		if (auto* param = getNamedParameter (paramState.name))
			param->setValueNotifyingHost (paramState.value);
		else
			jassertfalse;
	}
}

}  // namespace lemons::plugin
