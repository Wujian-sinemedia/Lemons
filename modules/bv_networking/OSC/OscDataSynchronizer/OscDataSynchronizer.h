#pragma once


namespace bav::network
{
class OscDataSynchronizerBase : public DataSynchronizer,
                                private OscListener
{
public:
    OscDataSynchronizerBase (SerializableData&       dataToUse,
                             juce::OSCSender&        s,
                             juce::OSCReceiver&      r,
                             std::function< void() > onConnectionLost = {});

private:
    void sendChangeData (const void* data, size_t dataSize) final;
    void oscMessageReceived (const juce::OSCMessage& message) final;

    const juce::OSCAddressPattern addressPattern;
    juce::OSCSender&              oscSender;
    juce::MemoryBlock             outgoingData;

    OscConnectionChecker connectionChecker;
    events::Listener     connectionListener;
};


struct OscDataSynchronizer : public OscManager
{
    OscDataSynchronizer (SerializableData&       dataToUse,
                         const String&           targetHostName   = "Host",
                         int                     portNumber       = 53100,
                         std::function< void() > onConnectionLost = {});

    virtual ~OscDataSynchronizer() = default;

private:
    OscDataSynchronizerBase sync;
};

}  // namespace bav::network
