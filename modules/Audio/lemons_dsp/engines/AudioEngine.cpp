
namespace lemons::dsp
{
template <typename SampleType>
void Engine<SampleType>::prepare (double samplerate, int blocksize, int numChannels)
{
	jassert (samplerate > 0 && blocksize > 0);
	dummyMidiBuffer.ensureSize (static_cast<size_t> (blocksize));
	outputStorage.setSize (numChannels, blocksize);
	hasBeenInitialized = true;
	sampleRate         = samplerate;
	prepared (blocksize, samplerate);
}

template <typename SampleType>
void Engine<SampleType>::releaseResources()
{
	dummyMidiBuffer.clear();
	hasBeenInitialized      = false;
	wasBypassedLastCallback = true;
	sampleRate              = 0.;
	released();
}

template <typename SampleType>
void Engine<SampleType>::process (AudioBuffer<SampleType>& inplaceInAndOut,
                                  MidiBuffer&              midiMessages,
                                  bool                     isBypassed)
{
	processInternal (inplaceInAndOut, inplaceInAndOut, midiMessages, isBypassed);
}

template <typename SampleType>
void Engine<SampleType>::process (const AudioBuffer<SampleType>& input,
                                  AudioBuffer<SampleType>&       output,
                                  MidiBuffer&                    midiMessages,
                                  bool                           isBypassed)
{
	processInternal (input, output, midiMessages, isBypassed);
}

template <typename SampleType>
void Engine<SampleType>::process (AudioBuffer<SampleType>& inplaceInAndOut,
                                  bool                     isBypassed)
{
	dummyMidiBuffer.clear();
	processInternal (inplaceInAndOut, inplaceInAndOut, dummyMidiBuffer, isBypassed);
}

template <typename SampleType>
void Engine<SampleType>::process (const AudioBuffer<SampleType>& input,
                                  AudioBuffer<SampleType>&       output,
                                  bool                           isBypassed)
{
	dummyMidiBuffer.clear();
	processInternal (input, output, dummyMidiBuffer, isBypassed);
}

template <typename SampleType>
void Engine<SampleType>::processInternal (const AudioBuffer<SampleType>& input, AudioBuffer<SampleType>& output, MidiBuffer& midiMessages, bool isBypassed)
{
	jassert (isInitialized() && sampleRate > 0);

	const bool applyFadeIn                 = wasBypassedLastCallback;
	const bool applyFadeOut                = isBypassed ? ! wasBypassedLastCallback : false;
	const bool processingBypassedThisFrame = isBypassed ? wasBypassedLastCallback : false;

	wasBypassedLastCallback = isBypassed;

	AudioBuffer<SampleType> alias { outputStorage.getArrayOfWritePointers(), outputStorage.getNumChannels(), 0, output.getNumSamples() };

	alias.clear();

	renderBlock (input, alias, midiMessages, processingBypassedThisFrame);

	if (applyFadeIn)
		alias.applyGainRamp (0, output.getNumSamples(), SampleType (0.), SampleType (1.));

	if (applyFadeOut)
		alias.applyGainRamp (0, output.getNumSamples(), SampleType (1.), SampleType (0.));

	buffers::copy (alias, output);
}

template <typename SampleType>
void Engine<SampleType>::prepared (int, double)
{
}


template class Engine<float>;
template class Engine<double>;

}  // namespace lemons::dsp
