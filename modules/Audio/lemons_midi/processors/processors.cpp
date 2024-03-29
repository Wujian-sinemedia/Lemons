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

namespace lemons::midi
{

void RoutingProcessor::process (MidiBuffer& block)
{
	std::for_each (block.begin(), block.end(),
				   [this] (const juce::MidiMessageMetadata& m)
				   { processMessage (m.getMessage()); });
}

void RoutingProcessor::processMessage (const MidiMessage& m)
{
	if (m.isNoteOn())
		return handleNoteOn (m.getNoteNumber(), m.getFloatVelocity());

	if (m.isNoteOff())
		return handleNoteOff (m.getNoteNumber(), m.getFloatVelocity());

	if (m.isPitchWheel())
		return handlePitchwheel (juce::jmap (m.getPitchWheelValue(), 0, 16383, 0, 127));

	if (m.isAftertouch())
		return handleAftertouch (m.getNoteNumber(), m.getAfterTouchValue());

	if (m.isChannelPressure())
		return handleChannelPressure (m.getChannelPressureValue());

	if (m.isAllNotesOff() || m.isAllSoundOff())
		return handleAllSoundOff();

	if (m.isResetAllControllers())
		return handleResetAllControllers();

	if (m.isController())
	{
		const auto controllerNumber = m.getControllerNumber();
		const auto controllerValue	= m.getControllerValue();

		switch (controllerNumber)
		{
			case 0x40 : return handleSustainPedal (controllerValue);
			case 0x42 : return handleSostenutoPedal (controllerValue);
			case 0x43 : return handleSoftPedal (controllerValue);
			default : return handleController (controllerNumber, controllerValue);
		}
	}

	if (m.isProgramChange())
		return handleProgramChange (m.getProgramChangeNumber());

	if (m.isSysEx())
		return handleSysEx (m.getSysExData(), m.getSysExDataSize());

	if (m.isSongPositionPointer())
		return handleSongPositionEvent (m.getSongPositionPointerMidiBeat());

	if (m.isTempoMetaEvent())
		return handleTempoMetaEvent (m.getTempoSecondsPerQuarterNote());

	if (m.isMidiMachineControlMessage())
		return handleMachineControl (m.getMidiMachineControlCommand());

	if (m.isTimeSignatureMetaEvent())
	{
		int numerator, denominator;
		m.getTimeSignatureInfo (numerator, denominator);
		return handleTimeSignatureMetaEvent (numerator, denominator);
	}

	if (m.isKeySignatureMetaEvent())
	{
		auto numSharps = m.getKeySignatureNumberOfSharpsOrFlats();

		const bool isSharps = numSharps > 0;

		if (! isSharps)
			numSharps = std::abs (numSharps);

		return handleKeySignatureMetaEvent (numSharps, isSharps,
											m.isKeySignatureMajorKey());
	}

	if (m.isTextMetaEvent())
		return handleTextMetaEvent (m.getTextFromTextMetaEvent());

	if (m.isMetaEvent())
		return handleMetaEvent (m.getMetaEventType(), m.getMetaEventData(), m.getMetaEventLength());

	handleOtherMessage (m);
}

void RoutingProcessor::handleNoteOn (int, float) { }
void RoutingProcessor::handleNoteOff (int, float) { }
void RoutingProcessor::handlePitchwheel (int) { }
void RoutingProcessor::handleAftertouch (int, int) { }
void RoutingProcessor::handleChannelPressure (int) { }
void RoutingProcessor::handleSustainPedal (int) { }
void RoutingProcessor::handleSostenutoPedal (int) { }
void RoutingProcessor::handleSoftPedal (int) { }
void RoutingProcessor::handleController (int, int) { }
void RoutingProcessor::handleProgramChange (int) { }
void RoutingProcessor::handleSysEx (const juce::uint8*, int) { }
void RoutingProcessor::handleMetaEvent (int, const juce::uint8*, int) { }
void RoutingProcessor::handleTextMetaEvent (String) { }
void RoutingProcessor::handleTempoMetaEvent (double) { }
void RoutingProcessor::handleTimeSignatureMetaEvent (int, int) { }
void RoutingProcessor::handleKeySignatureMetaEvent (int, bool, bool) { }
void RoutingProcessor::handleSongPositionEvent (int) { }


/*---------------------------------------------------------------------------------------------------------------------------------------*/


void StatefulRoutingProcessor::handlePitchwheel (int wheelValue)
{
	jassert (wheelValue >= 0 && wheelValue <= 127);
	pitchwheel = wheelValue;
	processPitchWheel (wheelValue);
}

int StatefulRoutingProcessor::getLastPitchwheelValue() const noexcept
{
	return pitchwheel;
}

void StatefulRoutingProcessor::handleSustainPedal (int controllerValue)
{
	jassert (controllerValue >= 0 && controllerValue <= 127);
	sustainPedalDown = controllerValue >= 64;
	processSustainPedal (controllerValue);
}

bool StatefulRoutingProcessor::isSustainPedalDown() const noexcept
{
	return sustainPedalDown;
}

void StatefulRoutingProcessor::handleSostenutoPedal (int controllerValue)
{
	jassert (controllerValue >= 0 && controllerValue <= 127);
	sostenutoPedalDown = controllerValue >= 64;
	processSostenutoPedal (controllerValue);
}

bool StatefulRoutingProcessor::isSostenutoPedalDown() const noexcept
{
	return sostenutoPedalDown;
}

void StatefulRoutingProcessor::handleSoftPedal (int controllerValue)
{
	jassert (controllerValue >= 0 && controllerValue <= 127);
	softPedalDown = controllerValue >= 64;
	processSoftPedal (controllerValue);
}

bool StatefulRoutingProcessor::isSoftPedalDown() const noexcept
{
	return softPedalDown;
}

void StatefulRoutingProcessor::reset()
{
	pitchwheel = 64;

	sustainPedalDown   = false;
	sostenutoPedalDown = false;
	softPedalDown	   = false;
}

}  // namespace lemons::midi
