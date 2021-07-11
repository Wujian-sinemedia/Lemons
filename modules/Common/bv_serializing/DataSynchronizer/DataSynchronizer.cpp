
namespace bav::serializing
{
DataSynchronizer::DataSynchronizer (SerializableData& dataToUse)
    : managedState (dataToUse)
{
}

void DataSynchronizer::applyRecievedData (const juce::MemoryBlock& recievedData)
{
    serializing::fromBinary (recievedData, managedState);
}

void DataSynchronizer::callback()
{
    juce::MemoryOutputStream os (cachedData, false);
    
    managedState.serialize().writeToStream (os);
    
    sendData (cachedData);
}

}  // namespace bav
