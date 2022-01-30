
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
 *  ======================================================================================
 */

namespace lemons::midi
{


void copyRangeOfMidiBuffer (const MidiBuffer& readingBuffer,
							MidiBuffer&		  destBuffer,
							int				  startSampleOfInput,
							int				  startSampleOfOutput,
							int				  numSamples)
{
	if (numSamples == 0) return;

	destBuffer.clear (startSampleOfOutput, numSamples);
	destBuffer.addEvents (readingBuffer,
						  startSampleOfInput,
						  numSamples,
						  startSampleOfOutput - startSampleOfInput);
}


ScopedMidiBufferAlias::ScopedMidiBufferAlias (MidiBuffer& originalBuffer,
											  MidiBuffer& aliasBufferToUse,
											  int		  startSampleInOrigBuffer,
											  int		  numSamplesToCopy)
	: origBuffer (originalBuffer), aliasBuffer (aliasBufferToUse), startSample (startSampleInOrigBuffer), numSamples (numSamplesToCopy)
{
	aliasBuffer.clear();
	copyRangeOfMidiBuffer (origBuffer, aliasBuffer, startSample, 0, numSamples);
}

ScopedMidiBufferAlias::~ScopedMidiBufferAlias()
{
	copyRangeOfMidiBuffer (aliasBuffer, origBuffer, 0, startSample, numSamples);
}


MidiBuffer midiBufferFromFile (const MidiFile& file, int trackToRead)
{
	MidiBuffer buffer;

	const auto addEventsFromTrack = [&buffer, &file] (int trackNum)
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

}  // namespace lemons::midi
