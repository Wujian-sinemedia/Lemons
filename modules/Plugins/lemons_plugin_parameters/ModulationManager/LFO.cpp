
namespace lemons::plugin
{
LFO::LFO (/*ParameterList& listToUse,*/ String metaParameterName)
    //: param (metaParameterName)
{
}

void LFO::prepareToPlay (int numSamples, double samplerate)
{
	storage.setSize (1, numSamples);
	osc.prepare (numSamples, samplerate);
}

[[nodiscard]] LFO::Osc& LFO::getOscillator() noexcept
{
	return osc;
}

[[nodiscard]] DefaultMetaParameter& LFO::getParameter() noexcept
{
	//return *param.get();
}

void LFO::addParameterTo (juce::AudioProcessor& processor)
{
	//param.addTo (processor);
}

}  // namespace lemons::plugin
