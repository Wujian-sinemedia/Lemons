#include <juce_audio_formats/juce_audio_formats.h>

namespace lemons::binary
{

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

		buffer.setSize (numChannels, numSamples);
		reader->read (&buffer, 0, numSamples, 0, true, numChannels > 1);
	}

	return buffer;
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


template <>
MemoryBlock audioToBinary (const AudioBuffer<float>& buffer)
{
	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };
	juce::FlacAudioFormat    format;

	if (auto* writer = format.createWriterFor (&stream, 48000, (unsigned) buffer.getNumChannels(), 24, {}, 0))
		writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());

	return block;
}

template <>
MemoryBlock audioToBinary (const AudioBuffer<double>& buffer)
{
	const auto numSamples  = buffer.getNumSamples();
	const auto numChannels = buffer.getNumChannels();

	AudioBuffer<float> floatBuf { numSamples, numChannels };

	for (int chan = 0; chan < numChannels; ++chan)
		vecops::convert (floatBuf.getWritePointer (chan), buffer.getReadPointer (chan), numSamples);

	return audioToBinary (floatBuf);
}


/*-------------------------------------------------------------------------------------------------------------------------------------*/


Image imageFromBinary (const MemoryBlock& block)
{
	juce::MemoryInputStream stream { block, false };
	juce::PNGImageFormat    format;

	return format.decodeImage (stream);
}

MemoryBlock imageToBinary (const Image& image)
{
	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };
	juce::PNGImageFormat     format;

	format.writeImageToStream (image, stream);

	return block;
}


/*-------------------------------------------------------------------------------------------------------------------------------------*/


MidiBuffer midiFromBinary (const MemoryBlock& block)
{
	juce::MemoryInputStream stream { block, false };

	juce::MidiFile file;
	file.readFrom (stream);

	if (auto* track = file.getTrack (0))
	{
		return [sequence = *track]() -> juce::MidiBuffer
		{
			juce::MidiBuffer buffer;

			for (const auto* holder : sequence)
				buffer.addEvent (holder->message,
				                 juce::roundToInt (holder->message.getTimeStamp()));

			return buffer;
		}();
	}

	return {};
}

MemoryBlock midiToBinary (const MidiBuffer& midi)
{
	juce::MidiFile file;

	const auto sequence = [&midi]() -> juce::MidiMessageSequence
	{
		juce::MidiMessageSequence seq;

		for (auto meta : midi)
			seq.addEvent (meta.getMessage());

		return seq;
	}();

	file.addTrack (sequence);

	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };

	file.writeTo (stream);

	return block;
}


/*-------------------------------------------------------------------------------------------------------------------------------------*/


String memoryBlockToString (const MemoryBlock& block)
{
	return block.toBase64Encoding();
}

MemoryBlock memoryBlockFromString (const String& string)
{
	juce::MemoryBlock block;
	block.fromBase64Encoding (string);
	return block;
}

}  // namespace lemons::binary
