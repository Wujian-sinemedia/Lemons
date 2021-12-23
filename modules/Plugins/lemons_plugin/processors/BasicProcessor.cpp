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

juce::AudioProcessor::BusesProperties BasicProcessor::getDefaultBusesLayout()
{
	const auto stereo = juce::AudioChannelSet::stereo();

	return BusesProperties()
	    .withInput (TRANS ("Input"), stereo, true)
	    .withOutput (TRANS ("Output"), stereo, true);
}

BasicProcessor::BasicProcessor (const BusesProperties& busesLayout)
    : AudioProcessor (busesLayout)
{
}

juce::AudioProcessorEditor* BasicProcessor::createEditor() { return new juce::GenericAudioProcessorEditor (*this); }

bool BasicProcessor::isBusesLayoutSupported (const BusesLayout& layout) const
{
	const auto disabled = juce::AudioChannelSet::disabled();

	return layout.getMainInputChannelSet() != disabled && layout.getMainOutputChannelSet() != disabled;
}

void BasicProcessor::repaintEditor() const
{
	juce::MessageManager::callAsync ([editor = juce::Component::SafePointer<juce::AudioProcessorEditor> (getActiveEditor())]
	                                 {
                                        if (auto* e = editor.getComponent())
                                            e->repaint(); });
}

}  // namespace lemons::dsp
