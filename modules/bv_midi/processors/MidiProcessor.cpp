
namespace bav::midi
{

void MidiProcessor::processMidiMessage (const Message& m)
{
    lastMidiChannel = m.getChannel();
    lastMidiTimestamp = static_cast<int> (m.getTimeStamp());
    
    if (m.isNoteOn())
        handleNoteOn (m.getNoteNumber(), m.getFloatVelocity());
    else if (m.isNoteOff())
        handleNoteOff (m.getNoteNumber(), m.getFloatVelocity());
    else if (m.isPitchWheel())
        handlePitchwheel (juce::jmap (m.getPitchWheelValue(), 0, 16383, 0, 127));
    else if (m.isAftertouch())
        handleAftertouch (m.getNoteNumber(), m.getAfterTouchValue());
    else if (m.isChannelPressure())
        handleChannelPressure (m.getChannelPressureValue());
    else if (m.isController())
        processControllerMessage (m.getControllerNumber(), m.getControllerValue());
    else if (m.isAllNotesOff() || m.isAllSoundOff())
        handleAllSoundOff();
}

void MidiProcessor::processControllerMessage (int controllerNumber, int controllerValue)
{
    lastMovedController = controllerNumber;
    lastControllerValue = controllerValue;
    
    switch (controllerNumber)
    {
        case 0x40 : handleSustainPedal (controllerValue); return;
        case 0x42 : handleSostenutoPedal (controllerValue); return;
        case 0x43 : handleSoftPedal (controllerValue); return;
        default : return;
    }
}

MidiProcessor::LastMovedControllerInfo MidiProcessor::getLastMovedCCinfo() const
{
    LastMovedControllerInfo info;
    info.controllerNumber = lastMovedController;
    info.controllerValue  = lastControllerValue;
    return info;
}

void MidiProcessor::handleNoteOn  (int, float) { }
void MidiProcessor::handleNoteOff (int, float) { }
void MidiProcessor::handlePitchwheel (int) { }
void MidiProcessor::handleAftertouch (int, int) { }
void MidiProcessor::handleChannelPressure (int) { }
void MidiProcessor::handleSustainPedal (int) { }
void MidiProcessor::handleSostenutoPedal (int) { }
void MidiProcessor::handleSoftPedal (int) { }

}  // namespace
