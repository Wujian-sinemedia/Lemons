
namespace MTSESP
{

void Client::Deleter::operator() (MTSClient* c)
{
	MTS_DeregisterClient (c);
}

bool Client::shouldFilterNote (int midiPitch, int midiChannel) const
{
	return MTS_ShouldFilterNote (client.get(), char (midiPitch), char (midiChannel));
}

bool Client::isConnected() const
{
	return MTS_HasMaster (client.get());
}

String Client::getScaleName() const
{
	return { MTS_GetScaleName (client.get()) };
}

void Client::parseMidiMessageForSysEx (const juce::MidiMessage& m) const
{
	if (! m.isSysEx())
		return;

	MTS_ParseMIDIDataU (client.get(),
	                    m.getSysExData(),
	                    m.getSysExDataSize());
}


}  // namespace MTSESP
