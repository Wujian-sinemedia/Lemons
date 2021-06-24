
namespace bav::network
{
IpcDataSynchronizer::IpcDataSynchronizer (SerializableData& dataToUse,
                                          std::function< void() >
                                              onConnectionLost)
    : DataSynchronizer (dataToUse), connectionLostFunc (onConnectionLost)
{
}

void IpcDataSynchronizer::sendChangeData (const void* data, size_t dataSize)
{
    block.replaceWith (data, dataSize);
    sendMessage (block);
}

void IpcDataSynchronizer::connectionMade()
{
}

void IpcDataSynchronizer::connectionLost()
{
    connectionLostFunc();
}

void IpcDataSynchronizer::messageReceived (const juce::MemoryBlock& message)
{
    applyChangeData (message.getData(), message.getSize());
}

}  // namespace bav::network
