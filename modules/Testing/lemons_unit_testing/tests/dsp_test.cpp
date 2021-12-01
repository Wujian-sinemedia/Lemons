namespace lemons::tests
{

DspTest::DspTest (const String& testName)
    : Test (testName, "DSP")
{
}

DspTest::DspTest (const String& testName, const String& testCategory)
    : Test (testName, testCategory)
{
    
}

template<typename SampleType>
String DspTest::getDspTestName (const String& name)
{
    if constexpr (std::is_same_v<SampleType, float>)
        return name + " (float)";
    else
        return name + " (double)";
}

template String DspTest::getDspTestName<float> (const String&);
template String DspTest::getDspTestName<double> (const String&);

void DspTest::fillMidiBufferWithRandomEvents (MidiBuffer& buffer, int numEvents)
{
    buffer.clear();
    
    auto rand = getRandom();
    
    for (int i = 0; i < numEvents; ++i)
        buffer.addEvent (juce::MidiMessage::controllerEvent (1, rand.nextInt (128), rand.nextInt (128)),
                        i);
}

template<typename SampleType>
void DspTest::fillAudioBufferWithRandomNoise (AudioBuffer<SampleType>& buffer)
{
    auto rand = getRandom();
    
    for (int chan = 0; chan < buffer.getNumChannels(); ++chan)
    {
        for (int s = 0; s < buffer.getNumSamples(); ++s)
        {
            buffer.setSample (chan, s,
                              static_cast<SampleType> (juce::jmap (rand.nextFloat(), -1.f, 1.f)));
        }
    }
}

template void DspTest::fillAudioBufferWithRandomNoise (AudioBuffer<float>&);
template void DspTest::fillAudioBufferWithRandomNoise (AudioBuffer<double>&);

template <typename FloatType>
bool DspTest::buffersAreEqual (const AudioBuffer<FloatType>& buffer1,
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

template bool DspTest::buffersAreEqual (const AudioBuffer<float>&, const AudioBuffer<float>&);
template bool DspTest::buffersAreEqual (const AudioBuffer<double>&, const AudioBuffer<double>&);

template<typename FloatType>
bool DspTest::buffersAreReasonablyEqual (const AudioBuffer<FloatType>& buffer1,
                                         const AudioBuffer<FloatType>& buffer2)
{
    jassert (buffer1.getNumChannels() == buffer2.getNumChannels());
    
    const auto numSamples = buffer1.getNumSamples();
    jassert (numSamples == buffer2.getNumSamples());
    
    const auto samplesAreWithinError = [](FloatType sample1, FloatType sample2, FloatType error)
    {
        return std::abs (sample1 - sample2) <= error;
    };
    
    for (int chan = 0; chan < buffer1.getNumChannels(); ++chan)
    {
        const auto* samplesA = buffer1.getReadPointer (chan);
        const auto* samplesB = buffer2.getReadPointer (chan);
        
        for (int s = 0; s < numSamples; ++s)
            if (! samplesAreWithinError (samplesA[s], samplesB[s], static_cast<FloatType>(0.001)))
                return false;
    }
    
    return true;
}

template bool DspTest::buffersAreReasonablyEqual (const AudioBuffer<float>&, const AudioBuffer<float>&);
template bool DspTest::buffersAreReasonablyEqual (const AudioBuffer<double>&, const AudioBuffer<double>&);

template <typename FloatType>
bool DspTest::allSamplesAreEqual (const juce::AudioBuffer<FloatType>& buffer1,
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

	for (int i1 = startIndex1, i2 = startIndex2;
	     i1 < startIndex1 + numSamples && i2 < startIndex2 + numSamples;
	     ++i1, ++i2)
	{
		if (buffer1.getSample (channel1, i1) != buffer2.getSample (channel2, i2))
			return false;
	}

	return true;
}

template bool DspTest::allSamplesAreEqual (const AudioBuffer<float>&, const AudioBuffer<float>&, int, int, int, int, int);
template bool DspTest::allSamplesAreEqual (const AudioBuffer<double>&, const AudioBuffer<double>&, int, int, int, int, int);

template <typename FloatType>
bool DspTest::allSamplesAreZero (const AudioBuffer<FloatType>& buffer,
                                 int startIndex, int numSamples, int channel)
{
	jassert (startIndex + numSamples <= buffer.getNumSamples());
	jassert (channel < buffer.getNumChannels());

	for (int i = startIndex; i < startIndex + numSamples; ++i)
		if (buffer.getSample (channel, i) != FloatType (0))
			return false;

	return true;
}

template bool DspTest::allSamplesAreZero (const AudioBuffer<float>&, int, int, int);
template bool DspTest::allSamplesAreZero (const AudioBuffer<double>&, int, int, int);


template <typename FloatType>
bool DspTest::bufferIsSilent (const AudioBuffer<FloatType>& buffer)
{
    const auto numSamples = buffer.getNumSamples();
    
    for (int chan = 0; chan < buffer.getNumChannels(); ++chan)
        if (! allSamplesAreZero (buffer, 0, numSamples, chan))
            return false;
    
    return true;
}

template bool DspTest::bufferIsSilent (const AudioBuffer<float>&);
template bool DspTest::bufferIsSilent (const AudioBuffer<double>&);


bool DspTest::midiBuffersAreEqual (const MidiBuffer& buffer1,
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


const std::vector<double>& DspTest::getTestingSamplerates()
{
	static const std::vector<double> samplerates { 44100., 44800., 96000. };
	return samplerates;
}

const std::vector<int>& DspTest::getTestingBlockSizes()
{
	static const std::vector<int> blocksizes { 256, 513, 1021, 2048 };
	return blocksizes;
}

}  // namespace lemons::tests
