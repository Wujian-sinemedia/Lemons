#pragma once


namespace bav::network
{
class OscDataSynchronizer : public DataSynchronizer,
                            private juce::OSCReceiver::Listener< juce::OSCReceiver::MessageLoopCallback >
{
public:
    OscDataSynchronizer (SerializableData& dataToUse, juce::OSCSender& s, juce::OSCReceiver& r);
    virtual ~OscDataSynchronizer() override;

private:
    void sendChangeData (const void* data, size_t dataSize) final;
    void oscMessageReceived (const juce::OSCMessage& message) final;

    const juce::OSCAddressPattern addressPattern;
    juce::OSCSender&              oscSender;
    juce::OSCReceiver&            oscReceiver;

    juce::MemoryBlock outgoingData;
};

}  // namespace bav::network
