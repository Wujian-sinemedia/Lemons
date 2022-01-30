
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

void MtsClient::Deleter::operator() (MTSClient* c)
{
	MTS_DeregisterClient (c);
}

bool MtsClient::shouldFilterNote (int midiPitch, int midiChannel) const
{
	return MTS_ShouldFilterNote (client.get(), char (midiPitch), char (midiChannel));
}

bool MtsClient::isConnected() const
{
	return MTS_HasMaster (client.get());
}

juce::String MtsClient::getScaleName() const
{
	return { MTS_GetScaleName (client.get()) };
}

void MtsClient::parseMidiMessageForSysEx (const juce::MidiMessage& m) const
{
	if (! m.isSysEx())
		return;

	MTS_ParseMIDIDataU (client.get(),
						m.getSysExData(),
						m.getSysExDataSize());
}


}  // namespace lemons::midi
