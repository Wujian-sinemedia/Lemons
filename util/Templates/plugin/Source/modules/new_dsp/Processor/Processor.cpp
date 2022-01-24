namespace New
{
Processor::Processor()
	: plugin::Processor<Engine, State> (lemons::plugin::BasicProcessor::BusesProperties()
											.withInput (TRANS ("Input"), juce::AudioChannelSet::stereo(), true)
											.withOutput (TRANS ("Output"), juce::AudioChannelSet::stereo(), true))
{
}

}  // namespace New
