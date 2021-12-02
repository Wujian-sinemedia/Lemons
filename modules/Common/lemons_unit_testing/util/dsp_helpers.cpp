namespace lemons::tests
{

void fillMidiBufferWithRandomEvents (MidiBuffer& buffer, int numEvents, juce::Random rng)
{
    buffer.clear();
    
    for (int i = 0; i < numEvents; ++i)
        buffer.addEvent (juce::MidiMessage::controllerEvent (1, rng.nextInt (128), rng.nextInt (128)),
                         i);
}

template <typename SampleType>
void fillAudioBufferWithRandomNoise (AudioBuffer<SampleType>& buffer, juce::Random rng)
{
    for (int chan = 0; chan < buffer.getNumChannels(); ++chan)
    {
        auto* samples = buffer.getWritePointer (chan);
        
        for (int s = 0; s < buffer.getNumSamples(); ++s)
            samples[s] = static_cast<SampleType> (juce::jmap (rng.nextFloat(), -1.f, 1.f));
    }
}

template void fillAudioBufferWithRandomNoise (AudioBuffer<float>&, juce::Random);
template void fillAudioBufferWithRandomNoise (AudioBuffer<double>&, juce::Random);

template <typename FloatType>
bool allSamplesAreEqual (const juce::AudioBuffer<FloatType>& buffer1,
                         const juce::AudioBuffer<FloatType>& buffer2,
                         int startIndex1, int numSamples, int startIndex2,
                         int channel1, int channel2)
{
	if (startIndex2 < 0) startIndex2 = startIndex1;
	if (channel2 < 0) channel2 = channel1;

	jassert (startIndex1 + numSamples <= buffer1.getNumSamples());
	jassert (startIndex2 + numSamples <= buffer2.getNumSamples());
	jassert (channel1 < buffer1.getNumChannels());
	jassert (channel2 < buffer2.getNumChannels());

	const auto* samplesA = buffer1.getReadPointer (channel1);
	const auto* samplesB = buffer2.getReadPointer (channel2);

	for (int i1 = startIndex1, i2 = startIndex2;
	     i1 < startIndex1 + numSamples && i2 < startIndex2 + numSamples;
	     ++i1, ++i2)
	{
		if (samplesA[i1] != samplesB[i2])
			return false;
	}

	return true;
}

template bool allSamplesAreEqual (const AudioBuffer<float>&, const AudioBuffer<float>&, int, int, int, int, int);
template bool allSamplesAreEqual (const AudioBuffer<double>&, const AudioBuffer<double>&, int, int, int, int, int);

template <typename FloatType>
bool bufferChannelsAreEqual (const AudioBuffer<FloatType>& buffer1,
                                           int channel1,
                                           const AudioBuffer<FloatType>& buffer2,
                                           int channel2)
{
    return allSamplesAreEqual (buffer1, buffer2, 0, buffer1.getNumSamples(), 0, channel1, channel2);
}

template bool bufferChannelsAreEqual (const AudioBuffer<float>&, int, const AudioBuffer<float>&, int);
template bool bufferChannelsAreEqual (const AudioBuffer<double>&, int, const AudioBuffer<double>&, int);

template <typename FloatType>
bool buffersAreEqual (const AudioBuffer<FloatType>& buffer1,
                      const AudioBuffer<FloatType>& buffer2)
{
	jassert (buffer1.getNumChannels() == buffer2.getNumChannels());

	const auto numSamples = buffer1.getNumSamples();
	jassert (numSamples == buffer2.getNumSamples());

	for (int chan = 0; chan < buffer1.getNumChannels(); ++chan)
		if (! allSamplesAreEqual (buffer1, buffer2, 0, numSamples, 0, chan, chan))
			return false;

	return true;
}

template bool buffersAreEqual (const AudioBuffer<float>&, const AudioBuffer<float>&);
template bool buffersAreEqual (const AudioBuffer<double>&, const AudioBuffer<double>&);

template <typename FloatType>
bool buffersAreReasonablyEqual (const AudioBuffer<FloatType>& buffer1,
                                const AudioBuffer<FloatType>& buffer2)
{
	jassert (buffer1.getNumChannels() == buffer2.getNumChannels());

	const auto numSamples = buffer1.getNumSamples();
	jassert (numSamples == buffer2.getNumSamples());

	const auto samplesAreWithinError = [] (FloatType sample1, FloatType sample2, FloatType error)
	{
		return std::abs (sample1 - sample2) <= error;
	};

	for (int chan = 0; chan < buffer1.getNumChannels(); ++chan)
	{
		const auto* samplesA = buffer1.getReadPointer (chan);
		const auto* samplesB = buffer2.getReadPointer (chan);

		for (int s = 0; s < numSamples; ++s)
			if (! samplesAreWithinError (samplesA[s], samplesB[s], static_cast<FloatType> (0.001)))
				return false;
	}

	return true;
}

template bool buffersAreReasonablyEqual (const AudioBuffer<float>&, const AudioBuffer<float>&);
template bool buffersAreReasonablyEqual (const AudioBuffer<double>&, const AudioBuffer<double>&);


template <typename FloatType>
bool allSamplesAreZero (const AudioBuffer<FloatType>& buffer,
                        int startIndex, int numSamples, int channel)
{
	jassert (startIndex + numSamples <= buffer.getNumSamples());
	jassert (channel < buffer.getNumChannels());

	const auto* samples = buffer.getReadPointer (channel);

	for (int i = startIndex; i < startIndex + numSamples; ++i)
		if (samples[i] != FloatType (0))
			return false;

	return true;
}

template bool allSamplesAreZero (const AudioBuffer<float>&, int, int, int);
template bool allSamplesAreZero (const AudioBuffer<double>&, int, int, int);


template <typename FloatType>
bool bufferIsSilent (const AudioBuffer<FloatType>& buffer)
{
	for (int chan = 0; chan < buffer.getNumChannels(); ++chan)
		if (! allSamplesAreZero (buffer, 0, buffer.getNumSamples(), chan))
			return false;

	return true;
}

template bool bufferIsSilent (const AudioBuffer<float>&);
template bool bufferIsSilent (const AudioBuffer<double>&);


template <typename FloatType>
bool allSamplesAreValid (const AudioBuffer<FloatType>& buffer)
{
	for (int chan = 0; chan < buffer.getNumChannels(); ++chan)
	{
		const auto* samples = buffer.getReadPointer (chan);

		for (int s = 0; s < buffer.getNumSamples(); ++s)
			if (std::isinf (samples[s]) || std::isnan (samples[s]))
				return false;
	}

	return true;
}

template bool allSamplesAreValid (const AudioBuffer<float>&);
template bool allSamplesAreValid (const AudioBuffer<double>&);


template <typename FloatType>
bool noSamplesAreClipping (const AudioBuffer<FloatType>& buffer)
{
    for (int chan = 0; chan < buffer.getNumChannels(); ++chan)
    {
        const auto* samples = buffer.getReadPointer (chan);
        
        for (int s = 0; s < buffer.getNumSamples(); ++s)
        {
            const auto sample = samples[s];
            
            if (sample > FloatType(1) || sample < FloatType(-1))
                return false;
        }
    }
    
    return true;
}

template bool noSamplesAreClipping (const AudioBuffer<float>&);
template bool noSamplesAreClipping (const AudioBuffer<double>&);


bool midiBuffersAreEqual (const MidiBuffer& buffer1,
                          const MidiBuffer& buffer2)
{
	if (buffer1.getNumEvents() != buffer2.getNumEvents())
		return false;

	if (buffer1.getFirstEventTime() != buffer2.getFirstEventTime())
		return false;

	if (buffer1.getLastEventTime() != buffer2.getLastEventTime())
		return false;

	for (auto it1 = buffer1.begin(), it2 = buffer2.begin();
	     it1 != buffer1.end() && it2 != buffer2.end();
	     ++it1, ++it2)
	{
		auto meta1 = *it1;
		auto meta2 = *it2;

		if (meta1.samplePosition != meta2.samplePosition)
			return false;

		if (meta1.getMessage().getDescription() != meta2.getMessage().getDescription())
			return false;
	}

	return true;
}

MidiBuffer makeCopyOfMidiBuffer (const MidiBuffer& inputBuffer)
{
    MidiBuffer newBuffer;
    
    if (const auto numSamples = inputBuffer.getLastEventTime();
        numSamples > 0)
    {
        newBuffer.addEvents (inputBuffer, 0, numSamples, 0);
        jassert (! newBuffer.isEmpty());
    }

    return newBuffer;
}

}  // namespace lemons::tests
