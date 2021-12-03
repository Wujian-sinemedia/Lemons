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
MemoryBlock audioToBinary (const AudioBuffer<float>& buffer, double samplerate)
{
    jassert (buffer.getNumChannels() > 0 && buffer.getNumSamples() > 0);
    
    MemoryBlock              block;
    juce::MemoryOutputStream stream { block, false };
    juce::FlacAudioFormat    format;
    
    if (auto* writer = format.createWriterFor (&stream, samplerate, static_cast<unsigned>(buffer.getNumChannels()), 24, {}, 0))
        writer->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());
    else
        jassertfalse;
    
    return block;
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
MemoryBlock audioToBinary (const AudioBuffer<double>& buffer, double samplerate)
{
	const auto numSamples  = buffer.getNumSamples();
	const auto numChannels = buffer.getNumChannels();

	AudioBuffer<float> floatBuf { numSamples, numChannels };

	for (int chan = 0; chan < numChannels; ++chan)
		vecops::convert (floatBuf.getWritePointer (chan), buffer.getReadPointer (chan), numSamples);

	return audioToBinary (floatBuf, samplerate);
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

MidiBuffer midiBufferFromMidiFile (const juce::MidiFile& file)
{
    if (const auto* track = file.getTrack (0))
    {
        return [sequence = *track]() -> MidiBuffer
        {
            MidiBuffer buffer;
            
            for (const auto* holder : sequence)
                buffer.addEvent (holder->message,
                                 juce::roundToInt (holder->message.getTimeStamp()));
            
            jassert (buffer.getNumEvents() == sequence.getNumEvents());
            
            return buffer;
        }();
    }
    
    jassertfalse;
    return {};
}

juce::MidiFile midiBufferToMidiFile (const MidiBuffer& midi)
{
    juce::MidiFile file;
    
    const auto sequence = [&]() -> juce::MidiMessageSequence
    {
        juce::MidiMessageSequence seq;
        
        for (const auto meta : midi)
            seq.addEvent (meta.getMessage());
        
        return seq;
    }();
    
    jassert (sequence.getNumEvents() == midi.getNumEvents());
    
    file.addTrack (sequence);
    
    jassert (file.getLastTimestamp() == midi.getLastEventTime());
    
    return file;
}


MidiBuffer midiFromBinary (const MemoryBlock& block)
{
	juce::MemoryInputStream stream { block, false };

	juce::MidiFile file;
	file.readFrom (stream);

    return midiBufferFromMidiFile (file);
}

MemoryBlock midiToBinary (const MidiBuffer& midi)
{
    const auto file = midiBufferToMidiFile (midi);

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
	MemoryBlock block;
	block.fromBase64Encoding (string);
	return block;
}


/*-------------------------------------------------------------------------------------------------------------------------------------*/


constexpr auto NAME_PROP     = "_name";
constexpr auto CHILDREN_PROP = "_children";
constexpr auto BASE64_PROP   = "_base64:";

juce::var valueTreeToVar (const ValueTree& tree)
{
	if (! tree.isValid())
		return {};

	juce::DynamicObject obj;

	obj.setProperty (NAME_PROP, tree.getType().toString());

	juce::Array<juce::var> children;

	for (const auto child : tree)
		if (const auto childVar = valueTreeToVar (child); ! childVar.isVoid())
			children.add (childVar);

	if (! children.isEmpty())
		obj.setProperty (CHILDREN_PROP, children);

	for (int i = 0; i < tree.getNumProperties(); i++)
	{
		const auto name = tree.getPropertyName (i).toString();
		const auto val  = tree.getProperty (name, {});

		if (const auto mb = val.getBinaryData())
		{
			obj.setProperty (BASE64_PROP + name, mb->toBase64Encoding());
			continue;
		}

		// These types can't be stored as JSON!
		jassert (! val.isObject());
		jassert (! val.isMethod());
		jassert (! val.isArray());

		obj.setProperty (name, val.toString());
	}

	return juce::var (&obj);
}

String valueTreeToJSON (const ValueTree& tree)
{
	return juce::JSON::toString (valueTreeToVar (tree));
}


ValueTree valueTreefromVar (const juce::var& obj)
{
	if (auto* dobj = obj.getDynamicObject(); dobj->hasProperty (NAME_PROP))
	{
		ValueTree tree { dobj->getProperty (NAME_PROP).toString() };

		if (const auto c = dobj->getProperty (CHILDREN_PROP); c.isArray())
			for (const auto& child : *c.getArray())
				if (const auto childTree = valueTreefromVar (child); childTree.isValid())
					tree.appendChild (childTree, nullptr);

		const auto base64PropLen = static_cast<int> (std::strlen (BASE64_PROP));

		for (auto itr : dobj->getProperties())
		{
			const auto name = itr.name.toString();

			if (name == NAME_PROP || name == CHILDREN_PROP) continue;

			if (name.startsWith (BASE64_PROP))
			{
				juce::MemoryBlock mb;

				if (mb.fromBase64Encoding (itr.value.toString()))
					tree.setProperty (name.substring (base64PropLen), juce::var (mb), nullptr);
			}
			else
			{
				tree.setProperty (name, juce::var (itr.value), nullptr);
			}
		}

		return tree;
	}

	return {};
}

ValueTree valueTreeFromJSON (const String& jsonText)
{
	return valueTreefromVar (juce::JSON::parse (jsonText));
}

}  // namespace lemons::binary


/*---------------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

DataConversionTests::DataConversionTests()
: CoreTest("Data conversion functions")
{ }

void DataConversionTests::runTest()
{
    /*
     - ValueTree to JSON
     - ValueTree from JSON
     */
    
//    beginTest ("Audio buffers");
//    runTypedTests<float>();
//    runTypedTests<double>();
    
    beginTest ("MIDI buffers");
    
    MidiBuffer origMidi;
    
    constexpr auto numMidiEvents = 256;
    
    fillMidiBufferWithRandomEvents (origMidi, numMidiEvents, getRandom());
    
    {
        const auto subtest = beginSubtest ("MIDI buffer to/from MIDI file");
        
        const auto file    = binary::midiBufferToMidiFile (origMidi);
        const auto decoded = binary::midiBufferFromMidiFile (file);
        
        expect (midiBuffersAreEqual (origMidi, decoded));
    }
    
    const auto block   = binary::midiToBinary (origMidi);
    const auto decoded = binary::midiFromBinary (block);
    
    //expect (midiBuffersAreEqual (origMidi, decoded));
    
    beginTest ("Images");
    
    constexpr auto imageWidth  = 250;
    constexpr auto imageHeight = 250;
    
    juce::Image image { juce::Image::PixelFormat::RGB, imageWidth, imageHeight, true };
    
    const auto blob = binary::imageToBinary (image);
    const auto decodedImage = binary::imageFromBinary (blob);
    
    expectEquals (decodedImage.getWidth(), imageWidth);
    expectEquals (decodedImage.getHeight(), imageHeight);
            
    
    beginTest ("Memory block to/from string");
    
    const auto memStr     = binary::memoryBlockToString (block);
    const auto memDecoded = binary::memoryBlockFromString (memStr);
    
    expect (block.matches (memDecoded.getData(), memDecoded.getSize()));
}

template<typename SampleType>
void DataConversionTests::runTypedTests()
{
    const auto precisionString = []() -> String
    {
        if constexpr (std::is_same_v<SampleType, float>)
            return "Float";
        else
            return "Double";
    }();
    
    const auto subtest = beginSubtest (precisionString + " precision tests");
    
    constexpr auto numChannels = 2;
    constexpr auto numSamples  = 512;
    
    AudioBuffer<SampleType> origAudio { numChannels, numSamples };
    
    fillAudioBufferWithRandomNoise (origAudio, getRandom());
    
    const auto block   = binary::audioToBinary (origAudio);
    const auto decoded = binary::audioFromBinary<SampleType> (block);

    expectEquals (decoded.getNumChannels(), numChannels);
    expectEquals (decoded.getNumSamples(), numSamples);

    expect (buffersAreReasonablyEqual (origAudio, decoded));
}

template void DataConversionTests::runTypedTests<float>();
template void DataConversionTests::runTypedTests<double>();

}

#endif
