
#pragma once

namespace bav::midi
{

struct MidiProcessor
{
    using Message = juce::MidiMessage;
    
    void processMidiMessage (const Message& m);
    
    int getLastMidiChannel() const { return lastMidiChannel; }
    int getLastMidiTimestamp() const { return lastMidiTimestamp; }
    
    struct LastMovedControllerInfo
    {
        int controllerNumber, controllerValue;
    };
    
    LastMovedControllerInfo getLastMovedCCinfo() const;
    
private:
    void processControllerMessage (int controllerNumber, int controllerValue);
    
    virtual void handleNoteOn  (int midiPitch, float velocity);
    virtual void handleNoteOff (int midiPitch, float velocity);
    virtual void handlePitchwheel (int wheelValue);
    virtual void handleAftertouch (int noteNumber, int aftertouchValue);
    virtual void handleChannelPressure (int channelPressureValue);
    virtual void handleSustainPedal (int controllerValue);
    virtual void handleSostenutoPedal (int controllerValue);
    virtual void handleSoftPedal (int controllerValue);
    virtual void handleAllSoundOff() { }
    
    int lastMidiChannel {1};
    int lastMidiTimestamp {0};
    
    int lastMovedController {0};
    int lastControllerValue {0};
};

}  // namespace
