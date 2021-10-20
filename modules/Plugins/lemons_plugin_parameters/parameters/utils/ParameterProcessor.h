#pragma once


namespace lemons::plugin
{
/** This class processes incoming MIDI messages and quantizes the audio output with any MIDI CC messages mapped to parameters.
    This works by processing the audio in chunks in between each MIDI message, so that the MIDI message processing is sample-accurate and synchronous. \n \n
    In order for this to work nicely, your parameter changes must be realtime safe, and your audio processing algorithm must be able to process as little as 1 sample at a time. \n \n
    This class inherits dsp::MidiChoppingProcessor but does not implement dsp::MidiChoppingProcessor::renderChunk() -- you should subclass this and implement that function to do your actual processing.
    @see dsp::MidiChoppingProcessor ParameterList
 */
template <typename SampleType>
class ParameterProcessor : public dsp::MidiChoppingProcessor<SampleType>
{
public:
	/** Creates a parameter processor.
	    @param listToUse The ParameterList to reference.
	 */
	ParameterProcessor (ParameterList& listToUse);

private:
	void handleMidiMessage (const MidiMessage& m) final;

	ParameterList& list;
};

}  // namespace lemons::plugin
