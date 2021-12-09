namespace lemons::dsp
{

template <typename SampleType>
AudioFilePlayer<SampleType>::AudioFilePlayer (AudioFile& file)
    : origAudio (file.getData<SampleType>())
    , origSamplerate (file.getSamplerate())
    , origNumSamples (file.getNumSamples())
{
	jassert (file.isValid());
	jassert (origNumSamples == origAudio.getNumSamples());

	interpolators.resize (origAudio.getNumChannels());
	readPositions.ensureStorageAllocated (origAudio.getNumChannels());
	readPositions.fill (0);
}

template <typename SampleType>
void AudioFilePlayer<SampleType>::renderChunk (const AudioBuffer<SampleType>&,
                                               AudioBuffer<SampleType>& output, MidiBuffer&, bool isBypassed)
{
	if (isBypassed)
	{
		for (auto* interp : interpolators)
			interp->reset();

		return;
	}

	const auto numSamples = output.getNumSamples();

	for (int chan = 0;
	     chan < std::min (interpolators->size(), output.getNumChannels());
	     ++chan)
	{
		const auto prevReadPos = readPositions.getUnchecked (chan);

		const auto numUsed = interpolators[chan]->process (speedRatio,
		                                                   origAudio.getReadPointer (chan, prevReadPos),
		                                                   output.getWritePointer (chan),
		                                                   numSamples,
		                                                   origNumSamples - prevReadPos,
		                                                   0);

		readPositions.set (chan, prevReadPos + numUsed);
	}
}

template <typename SampleType>
void AudioFilePlayer<SampleType>::onPrepare (int blocksize, double samplerate, int)
{
	jassert (blocksize == this->reportLatency());

	constexpr auto baseLatency = InterpolatorType::getBaseLatency();

	speedRatio = origSamplerate / samplerate;

	jassert (speedRatio > 0.);

	const auto latency = juce::roundToInt (baseLatency / speedRatio);

	if (latency != this->reportLatency())
		this->changeLatency (latency);
}

template <typename SampleType>
void AudioFilePlayer<SampleType>::onRelease()
{
	for (int i = 0; i < interpolators->size(); ++i)
	{
		interpolators[i]->reset();
		readPositions.set (i, 0);
	}
}

template class AudioFilePlayer<float>;
// template class AudioFilePlayer<double>;

}  // namespace lemons::dsp
