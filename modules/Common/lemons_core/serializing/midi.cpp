namespace lemons::serializing
{

MidiBuffer midiBufferFromFile (const juce::MidiFile& file, int trackToRead)
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

juce::MidiFile midiBufferToFile (const MidiBuffer& midi)
{
	juce::MidiMessageSequence seq;

	for (const auto& meta : midi)
		seq.addEvent (meta.getMessage());

	jassert (seq.getNumEvents() == midi.getNumEvents());

	juce::MidiFile file;

	file.addTrack (seq);

	jassert (file.getLastTimestamp() == midi.getLastEventTime());

	return file;
}

MidiBuffer midiFromBinary (const MemoryBlock& block)
{
	juce::MemoryInputStream stream { block, false };

	juce::MidiFile file;
	file.readFrom (stream);

	return midiBufferFromFile (file);
}

MemoryBlock midiToBinary (const MidiBuffer& midi)
{
	const auto file = midiBufferToFile (midi);

	MemoryBlock              block;
	juce::MemoryOutputStream stream { block, false };

	file.writeTo (stream);

	return block;
}

}  // namespace lemons::serializing
