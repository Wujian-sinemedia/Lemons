namespace lemons::serializing
{

template <>
MemoryBlock audioToBinary (const AudioBuffer<float>& buffer, double samplerate)
{
	jassert (buffer.getNumChannels() > 0 && buffer.getNumSamples() > 0);

	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };
	juce::FlacAudioFormat    format;

	if (auto* writer = format.createWriterFor (&stream, samplerate, static_cast<unsigned> (buffer.getNumChannels()), 24, {}, 0))
		writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());
	else
		jassertfalse;

	return block;
}

template <>
AudioBuffer<float> audioFromBinary (const MemoryBlock& block)
{
	AudioBuffer<float> buffer;

	juce::MemoryInputStream in { block.getData(), block.getSize(), false };
	juce::FlacAudioFormat   format;

	if (auto* reader = format.createReaderFor (&in, false))
	{
		const auto numChannels = static_cast<int> (reader->numChannels);
		const auto numSamples  = static_cast<int> (reader->lengthInSamples);

		jassert (numChannels > 0 && numSamples > 0);

		buffer.setSize (numChannels, numSamples);
		reader->read (&buffer, 0, numSamples, 0, true, numChannels > 1);
	}
	else
	{
		jassertfalse;
	}

	return buffer;
}

template <>
MemoryBlock audioToBinary (const AudioBuffer<double>& buffer, double samplerate)
{
	const auto numSamples  = buffer.getNumSamples();
	const auto numChannels = buffer.getNumChannels();

	AudioBuffer<float> floatBuf { numSamples, numChannels };

	for (int chan = 0; chan < numChannels; ++chan)
		vecops::convert (floatBuf.getWritePointer (chan), buffer.getReadPointer (chan), numSamples);

	return audioToBinary (floatBuf, samplerate);
}

template <>
AudioBuffer<double> audioFromBinary (const MemoryBlock& block)
{
	const auto floatBuf = audioFromBinary<float> (block);

	const auto numSamples  = floatBuf.getNumSamples();
	const auto numChannels = floatBuf.getNumChannels();

	AudioBuffer<double> doubleBuf { numSamples, numChannels };

	for (int chan = 0; chan < numChannels; ++chan)
		vecops::convert (doubleBuf.getWritePointer (chan), floatBuf.getReadPointer (chan), numSamples);

	return doubleBuf;
}


template <typename SampleType>
bool saveAudioToFile (const AudioBuffer<SampleType>& buffer, const File& file, double samplerate)
{
	return files::saveBlockToFile (audioToBinary (buffer, samplerate), file);
}

template <typename SampleType>
AudioBuffer<SampleType> loadAudioFromFile (const File& file)
{
	return audioFromBinary<SampleType> (files::loadFileAsBlock (file));
}

}  // namespace lemons::serializing

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

using namespace lemons::serializing;

ADSR::Parameters VariantConverter<ADSR::Parameters>::fromVar (const var& v)
{
	ADSR::Parameters p;

	if (const auto* obj = v.getDynamicObject())
	{
		if (obj->hasProperty (attack))
			p.attack = obj->getProperty (attack);

		if (obj->hasProperty (decay))
			p.decay = obj->getProperty (decay);

		if (obj->hasProperty (sustain))
			p.sustain = obj->getProperty (sustain);

		if (obj->hasProperty (release))
			p.release = obj->getProperty (release);
	}

	return p;
}

var VariantConverter<ADSR::Parameters>::toVar (const ADSR::Parameters& p)
{
	DynamicObject obj;

	obj.setProperty (attack, p.attack);
	obj.setProperty (decay, p.decay);
	obj.setProperty (sustain, p.sustain);
	obj.setProperty (release, p.release);

	return { obj.clone().get() };
}

AudioBuffer<float> VariantConverter<AudioBuffer<float>>::fromVar (const var& v)
{
	return audioFromBinary<float> (memoryBlockFromString (v.toString()));
}

var VariantConverter<AudioBuffer<float>>::toVar (const AudioBuffer<float>& b)
{
	return { memoryBlockToString (audioToBinary (b)) };
}

AudioBuffer<double> VariantConverter<AudioBuffer<double>>::fromVar (const var& v)
{
	return audioFromBinary<double> (memoryBlockFromString (v.toString()));
}

var VariantConverter<AudioBuffer<double>>::toVar (const AudioBuffer<double>& b)
{
	return { memoryBlockToString (audioToBinary (b)) };
}

PluginDescription VariantConverter<PluginDescription>::fromVar (const var& v)
{
	PluginDescription d;

	d.loadFromXml (VariantConverter<XmlElement>::fromVar (v));

	return d;
}

var VariantConverter<PluginDescription>::toVar (const PluginDescription& d)
{
	return VariantConverter<XmlElement>::toVar (*d.createXml());
}

}  // namespace juce

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::tests
{

DspSerializingTests::DspSerializingTests()
    : CoreTest ("DSP serializing")
{
}

void DspSerializingTests::runTest()
{
	// PluginDescription

	auto rand = getRandom();

	{
		const auto subtest = beginSubtest ("ADSR::Parameters");

		const juce::ADSR::Parameters params { rand.nextFloat(), rand.nextFloat(), rand.nextFloat(), rand.nextFloat() };

		const auto decoded = toVarAndBack (params);

		expectEquals (decoded.attack, params.attack);
		expectEquals (decoded.decay, params.decay);
		expectEquals (decoded.sustain, params.sustain);
		expectEquals (decoded.release, params.release);
	}

	{
		const auto subtest = beginSubtest ("Audio buffers");

		runAudioBufferTest<float>();
		runAudioBufferTest<double>();
	}
}

template <typename SampleType>
void DspSerializingTests::runAudioBufferTest()
{
	const auto subtest = beginSubtest (getPrecisionString<SampleType>() + " precision tests");

	constexpr auto numChannels = 2;
	constexpr auto numSamples  = 512;

	AudioBuffer<SampleType> origAudio { numChannels, numSamples };

	fillAudioBufferWithRandomNoise (origAudio, getRandom());

	//    expect (buffersAreReasonablyEqual (origAudio, toVarAndBack (origAudio)));

	//    const auto block   = serializing::audioToBinary (origAudio);
	//    const auto decoded = serializing::audioFromBinary<SampleType> (block);
	//
	//    expectEquals (decoded.getNumChannels(), numChannels);
	//    expectEquals (decoded.getNumSamples(), numSamples);
	//
	//    expect (buffersAreReasonablyEqual (origAudio, decoded));
}

template void DspSerializingTests::runAudioBufferTest<float>();
template void DspSerializingTests::runAudioBufferTest<double>();

}  // namespace lemons::tests
