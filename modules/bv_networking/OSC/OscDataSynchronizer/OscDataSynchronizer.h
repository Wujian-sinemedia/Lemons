#pragma once


namespace bav::network
{
class OscDataSynchronizerBase : public DataSynchronizer,
                                private juce::OSCReceiver::Listener< juce::OSCReceiver::MessageLoopCallback >
{
public:
    OscDataSynchronizerBase (SerializableData& dataToUse, juce::OSCSender& s, juce::OSCReceiver& r);
    virtual ~OscDataSynchronizerBase() override;

private:
    void sendChangeData (const void* data, size_t dataSize) final;
    void oscMessageReceived (const juce::OSCMessage& message) final;

    const juce::OSCAddressPattern addressPattern;
    juce::OSCSender&              oscSender;
    juce::OSCReceiver&            oscReceiver;

    juce::MemoryBlock outgoingData;
};


struct OscDataSynchronizer : public OscManager
{
    OscDataSynchronizer (SerializableData& dataToUse, const String& targetHostName = "Host", int portNumber = 53100);

    virtual ~OscDataSynchronizer() = default;

private:
    OscDataSynchronizerBase sync;
};

}  // namespace bav::network
