#pragma once

namespace bav::network
{
struct OscListener : public juce::OSCReceiver::Listener< juce::OSCReceiver::MessageLoopCallback >
{
    OscListener (juce::OSCReceiver& receiverToUse);

    virtual ~OscListener() override;

private:
    juce::OSCReceiver& receiver;
};

}  // namespace bav::network
