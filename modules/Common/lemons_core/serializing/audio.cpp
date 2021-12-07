#include <juce_audio_formats/juce_audio_formats.h>

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

}  // namespace lemons::serializing
