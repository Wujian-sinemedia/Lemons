#pragma once

namespace bav::network
{
class IpcDataSynchronizer : public juce::InterprocessConnection, public DataSynchronizer
{
public:
    IpcDataSynchronizer (SerializableData&       dataToUse,
                         std::function< void() > onConnectionLost = {});

private:
    void sendChangeData (const void* data, size_t dataSize) final;

    void connectionMade() final;
    void connectionLost() final;
    void messageReceived (const juce::MemoryBlock& message) final;

    juce::MemoryBlock       block;
    std::function< void() > connectionLostFunc;
};

}  // namespace bav::network
