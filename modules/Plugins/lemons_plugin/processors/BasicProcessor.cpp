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

void BasicProcessor::processBlockBypassed (AudioBuffer<float>& audio, MidiBuffer&)
{
	audio.clear();
}

void BasicProcessor::processBlockBypassed (AudioBuffer<double>& audio, MidiBuffer&)
{
	audio.clear();
}

bool BasicProcessor::isBusesLayoutSupported (const BusesLayout& layout) const
{
	const auto disabled = juce::AudioChannelSet::disabled();

	return layout.getMainInputChannelSet() != disabled && layout.getMainOutputChannelSet() != disabled;
}

void BasicProcessor::callEditorMethod (std::function<void (juce::AudioProcessorEditor&)> func) const
{
	if (! hasEditor())
		return;

	juce::MessageManager::callAsync ([editor = juce::Component::SafePointer<juce::AudioProcessorEditor> (getActiveEditor()), func]
	                                 {
        if (auto* e = editor.getComponent())
            func (*e); });
}

void BasicProcessor::repaintEditor() const
{
	callEditorMethod ([&] (juce::AudioProcessorEditor& e)
	                  { e.repaint(); });
}

}  // namespace lemons::plugin
