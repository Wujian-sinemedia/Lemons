#pragma once

namespace bav::network
{
struct OscListener : public juce::OSCReceiver::Listener< juce::OSCReceiver::MessageLoopCallback >
{
    OscListener (juce::OSCReceiver& receiverToUse)
        : receiver (receiverToUse)
    {
        receiver.addListener (this);
    }

    virtual ~OscListener() override
    {
        receiver.removeListener (this);
    }

private:
    juce::OSCReceiver& receiver;
};

}  // namespace bav::network
