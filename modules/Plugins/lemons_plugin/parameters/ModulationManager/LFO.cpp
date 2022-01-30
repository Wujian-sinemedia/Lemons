
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 *  ======================================================================================
 */

namespace lemons::plugin
{

LFO::LFO (const ParameterList& parameterListToUse, const String& paramName)
	: MetaParameter<float> (
		parameterListToUse,
		0.f, 1.f, 0.f, paramName,
		nullptr, nullptr, {}, false)
{
}

void LFO::prepareToPlay (int numSamples, double samplerate)
{
	storage.setSize (1, numSamples, true, true, true);
	osc.prepare (numSamples, samplerate, 1);
}

void LFO::finishBlock (int numSamples)
{
	storage.clear();

	auto alias = dsp::buffers::getAliasBuffer (storage, 0, numSamples);

	osc.process (alias);
	readIdx = numSamples - 1;

	setNormalizedValue (juce::jmap (getCurrentOscillatorValue(), -1.f, 1.f, 0.f, 1.f));
}

float LFO::getCurrentOscillatorValue() const noexcept
{
	jassert (readIdx >= 0 && readIdx <= storage.getNumSamples() && ! storage.hasBeenCleared());
	return storage.getSample (0, readIdx);
}

typename LFO::Osc& LFO::getOscillator() noexcept
{
	return osc;
}


namespace LfoVTprops
{
static constexpr auto freqProp = "frequency";
static constexpr auto typeProp = "wave_type";
}  // namespace LfoVTprops

ValueTree LFO::saveToValueTree() const
{
	auto tree = MetaParameter<float>::saveToValueTree();

	using namespace LfoVTprops;

	tree.setProperty (freqProp, osc.getFrequency(), nullptr);
	tree.setProperty (typeProp, static_cast<int> (osc.getOscType()), nullptr);

	return tree;
}

void LFO::loadFromValueTree (const ValueTree& tree)
{
	if (! tree.hasType (Parameter::valueTreeType))
		return;

	MetaParameter<float>::loadFromValueTree (tree);

	using namespace LfoVTprops;

	if (tree.hasProperty (freqProp))
		osc.setFrequency ((float) tree.getProperty (freqProp));	 // NOLINT

	if (tree.hasProperty (typeProp))
		osc.setOscType (static_cast<dsp::osc::OscType> ((int) tree.getProperty (typeProp)));  // NOLINT
}

}  // namespace lemons::plugin
