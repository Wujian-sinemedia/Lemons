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


namespace lemons::serializing
{

MidiBuffer midiBufferFromFile (const MidiFile& file, int trackToRead)
{
	MidiBuffer buffer;

	const auto addEventsFromTrack = [&] (int trackNum)
	{
		if (const auto* track = file.getTrack (trackNum))
		{
			for (const auto* holder : *track)
				buffer.addEvent (holder->message,
				                 juce::roundToInt (holder->message.getTimeStamp()));
		}
	};

	if (trackToRead > -1)
	{
		addEventsFromTrack (trackToRead);
	}
	else
	{
		for (int i = 0; i < file.getNumTracks(); ++i)
			addEventsFromTrack (i);
	}

	jassert (juce::roundToInt (file.getLastTimestamp()) == buffer.getLastEventTime());

	return buffer;
}

MidiFile midiBufferToFile (const MidiBuffer& midi)
{
	juce::MidiMessageSequence seq;

	for (const auto& meta : midi)
		seq.addEvent (meta.getMessage());

	jassert (seq.getNumEvents() == midi.getNumEvents());

	MidiFile file;

	file.addTrack (seq);

	jassert (file.getLastTimestamp() == midi.getLastEventTime());

	return file;
}

MidiBuffer midiBufferFromBinary (const MemoryBlock& block)
{
	return midiBufferFromFile (midiFileFromBinary (block));
}

MidiFile midiFileFromBinary (const MemoryBlock& block)
{
	juce::MemoryInputStream stream { block, false };

	MidiFile file;
	file.readFrom (stream);

	return file;
}

MemoryBlock midiToBinary (const MidiBuffer& midi)
{
	return midiToBinary (midiBufferToFile (midi));
}

MemoryBlock midiToBinary (const MidiFile& midi)
{
	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };

	midi.writeTo (stream);

	return block;
}

bool saveMidiToFile (const MidiFile& midi, const File& file)
{
	if (auto os = file.createOutputStream())
		if (os->openedOk())
			return midi.writeTo (*os);

	return false;
}

bool saveMidiToFile (const MidiBuffer& midi, const File& file)
{
	return saveMidiToFile (midiBufferToFile (midi), file);
}

MidiFile loadMidiFromFile (const File& file)
{
	MidiFile midi;

	if (auto is = file.createInputStream())
		if (is->openedOk())
			midi.readFrom (*is);

	return midi;
}

MidiBuffer loadMidiBufferFromFile (const File& file)
{
	return midiBufferFromFile (loadMidiFromFile (file));
}

}  // namespace lemons::serializing

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::binary
{

juce::MidiFile getMidiFile (const String& midiFileName)
{
	return serializing::midiFileFromBinary (getBlob (midiFileName));
}

juce::MidiBuffer getMidiBuffer (const String& midiFileName)
{
	return serializing::midiBufferFromBinary (getBlob (midiFileName));
}

}  // namespace lemons::binary

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace juce
{

using namespace lemons::serializing;

MidiBuffer VariantConverter<MidiBuffer>::fromVar (const var& v)
{
	return midiBufferFromBinary (memoryBlockFromString (v.toString()));
}

var VariantConverter<MidiBuffer>::toVar (const MidiBuffer& b)
{
	return { memoryBlockToString (midiToBinary (b)) };
}

MidiFile VariantConverter<MidiFile>::fromVar (const var& v)
{
	juce::MidiFile file;

	if (const auto* block = v.getBinaryData())
	{
		juce::MemoryInputStream stream { *block, false };
		file.readFrom (stream);
	}

	return file;
}

var VariantConverter<MidiFile>::toVar (const MidiFile& f)
{
	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };

	f.writeTo (stream);

	return { block };
}

MidiMessage VariantConverter<MidiMessage>::fromVar (const var& v)
{
	if (const auto* obj = v.getDynamicObject())
	{
		if (obj->hasProperty (data_prop))
		{
			if (const auto* data = obj->getProperty (data_prop).getBinaryData())
			{
				const auto timestamp = [&]() -> double
				{
					if (obj->hasProperty (time_prop))
						return obj->getProperty (time_prop);

					return 0.;
				}();

				return { data->getData(), static_cast<int> (data->getSize()), timestamp };
			}
		}
	}

	return {};
}

var VariantConverter<MidiMessage>::toVar (const MidiMessage& m)
{
	DynamicObject obj;

	MemoryBlock block { m.getRawData(), static_cast<size_t> (m.getRawDataSize()) };

	obj.setProperty (data_prop, block);
	obj.setProperty (time_prop, m.getTimeStamp());

	return { obj.clone().get() };
}

}  // namespace juce

/*---------------------------------------------------------------------------------------------------------------------------*/

#if LEMONS_UNIT_TESTS

namespace lemons::tests
{

MidiSerializingTests::MidiSerializingTests()
    : CoreTest ("MIDI serializing")
{
}

void MidiSerializingTests::runTest()
{
	MidiBuffer origMidi;

	fillMidiBufferWithRandomEvents (origMidi, 256, getRandom());

	{
		const auto subtest = beginSubtest ("MIDI buffer to/from MIDI file");

		const auto file    = serializing::midiBufferToFile (origMidi);
		const auto decoded = serializing::midiBufferFromFile (file);

		expect (midiBuffersAreEqual (origMidi, decoded));
	}

	{
		const auto block   = serializing::midiToBinary (origMidi);
		const auto decoded = serializing::midiBufferFromBinary (block);

		//        expect (midiBuffersAreEqual (origMidi, decoded));
	}

	{
		const auto subtest = beginSubtest ("MidiBuffer");

		MidiBuffer orig;

		fillMidiBufferWithRandomEvents (orig, getRandom().nextInt ({ 1, 100 }), getRandom());

		//        expect (midiBuffersAreEqual (orig, toVarAndBack (orig)));
	}

	//    {
	//        const auto subtest = beginSubtest ("MidiFile");
	//    }

	{
		const auto subtest = beginSubtest ("MidiMessage");

		constexpr auto channel  = 1;
		constexpr auto note     = 78;
		constexpr auto velocity = 0.38f;

		const auto orig = juce::MidiMessage::noteOn (channel, note, velocity);

		const auto decoded = toVarAndBack (orig);

		expect (decoded.isNoteOn());

		expectEquals (decoded.getChannel(), channel);
		expectEquals (decoded.getNoteNumber(), note);

		expectWithinAbsoluteError (decoded.getFloatVelocity(), velocity, 0.01f);
	}
}

}  // namespace lemons::tests

#endif
