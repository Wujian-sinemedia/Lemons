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


namespace lemons::dsp
{

template <typename SampleType>
bool Engine<SampleType>::isInitialized() const noexcept
{
	return sampleRate > 0. && blockSize > 0;
}

template <typename SampleType>
int Engine<SampleType>::reportLatency() const noexcept
{
	return 0;
}

template <typename SampleType>
double Engine<SampleType>::getSamplerate() const noexcept
{
	return sampleRate;
}

template <typename SampleType>
int Engine<SampleType>::getBlocksize() const noexcept
{
	return blockSize;
}

template <typename SampleType>
void Engine<SampleType>::prepare (double samplerate, int blocksize, int numChannels)
{
	jassert (samplerate > 0 && blocksize > 0 && numChannels > 0);

	dummyMidiBuffer.ensureSize (static_cast<size_t> (blocksize));
	outputStorage.setSize (numChannels, blocksize, true, true, true);
	sampleRate = samplerate;
	blockSize  = blocksize;

	prepared (blocksize, samplerate, numChannels);
}

template <typename SampleType>
void Engine<SampleType>::releaseResources()
{
	dummyMidiBuffer.clear();
	outputStorage.setSize (0, 0);
	wasBypassedLastCallback = true;
	sampleRate              = 0.;
	blockSize               = 0;

	released();
}

template <typename SampleType>
int Engine<SampleType>::getNumChannels() const noexcept
{
	return outputStorage.getNumChannels();
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
	const auto numSamples = output.getNumSamples();
	jassert (numSamples == input.getNumSamples());

	if (numSamples == 0)
		return;

	if (input.getNumChannels() == 0 || output.getNumChannels() == 0)
		return;

	jassert (isInitialized());

	bool applyFadeIn                 = false;
	bool applyFadeOut                = false;
	bool processingBypassedThisFrame = isBypassed;

	if (isBypassed && ! wasBypassedLastCallback)
	{
		applyFadeOut                = true;
		processingBypassedThisFrame = false;
	}
	else if (wasBypassedLastCallback && ! isBypassed)
	{
		applyFadeIn                 = true;
		processingBypassedThisFrame = false;
	}

	jassert (! (applyFadeIn && applyFadeOut));

	auto alias = buffers::getAliasBuffer (outputStorage, 0, numSamples);

	alias.clear();

	renderBlock (input, alias, midiMessages, processingBypassedThisFrame);

	if (applyFadeIn)
		alias.applyGainRamp (0, numSamples, SampleType (0), SampleType (1));
	else if (applyFadeOut)
		alias.applyGainRamp (0, numSamples, SampleType (1), SampleType (0));

	buffers::copy (alias, output);

	wasBypassedLastCallback = isBypassed;
}

template <typename SampleType>
void Engine<SampleType>::reset()
{
	wasBypassedLastCallback = true;
	onReset();
}

template <typename SampleType>
void Engine<SampleType>::prepared (int, double, int)
{
}


template class Engine<float>;
template class Engine<double>;


#if LEMONS_UNIT_TESTS

template <typename SampleType>
void PassThroughEngine<SampleType>::renderBlock (const AudioBuffer<SampleType>& input,
                                                 AudioBuffer<SampleType>&       output,
                                                 MidiBuffer& midiMessages, bool isBypassed)
{
	if (isBypassed)
		output.clear();
	else
		buffers::copy (input, output);
}

template class PassThroughEngine<float>;
template class PassThroughEngine<double>;

#endif

}  // namespace lemons::dsp
