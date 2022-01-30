
/*
 * ======================================================================================
 *
 *  ██╗     ███████╗███╗   ███╗ ██████╗ ███╗   ██╗███████╗
 *  ██║     ██╔════╝████╗ ████║██╔═══██╗████╗  ██║██╔════╝
 *  ██║     █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║███████╗
 *  ██║     ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║╚════██║
 *  ███████╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║███████║
 *  ╚══════╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝╚══════╝
 *
 *  This file is part of the Lemons open source library and is licensed under the terms of the GNU Public License.
 *
 * ======================================================================================
 */

namespace lemons::serializing
{


MidiBuffer midiBufferFromBinary (const MemoryBlock& block)
{
	return midi::midiBufferFromFile (midiFileFromBinary (block));
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
	return midiToBinary (midi::midiBufferToFile (midi));
}

MemoryBlock midiToBinary (const MidiFile& midi)
{
	MemoryBlock				 block;
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
	return saveMidiToFile (midi::midiBufferToFile (midi), file);
}

}  // namespace lemons::serializing

/*---------------------------------------------------------------------------------------------------------------------------*/

namespace lemons::files
{

MidiFile loadMidiFile (const File& file)
{
	MidiFile midi;

	if (auto is = file.createInputStream())
		if (is->openedOk())
			midi.readFrom (*is);

	return midi;
}

MidiBuffer loadMidiBuffer (const File& file)
{
	return midi::midiBufferFromFile (loadMidiFile (file));
}

bool saveMidi (const MidiFile& midi, const File& file)
{
	return saveBlockToFile (serializing::midiToBinary (midi), file);
}

bool saveMidi (const MidiBuffer& midi, const File& file)
{
	return saveBlockToFile (serializing::midiToBinary (midi), file);
}


static const juce::StringArray& getMidiFileExtensions()
{
	static const juce::StringArray midiFileExtensions { ".mid", ".rmi", ".rmid", ".midi" };

	return midiFileExtensions;
}

bool isMidiFile (const File& file)
{
	for (const auto& xtn : getMidiFileExtensions())
		if (file.hasFileExtension (xtn))
			return true;

	return false;
}

}  // namespace lemons::files

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


juce::StringArray getMidiFileNames()
{
	juce::StringArray midiFiles;

	for (const auto& filename : getFilenames())
	{
		const auto isMidi = [&filename]
		{
			for (const auto& xtn : files::getMidiFileExtensions())
				if (filename.endsWith (xtn))
					return true;

			return false;
		}();

		if (isMidi)
			midiFiles.add (filename);
	}

	return midiFiles;
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
	MemoryBlock				 block;
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
				const auto timestamp = [&obj]() -> double
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
