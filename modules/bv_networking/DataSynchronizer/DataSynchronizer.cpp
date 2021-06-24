
namespace bav
{
DataSynchronizer::DataSynchronizer (SerializableData& dataToUse)
    : sData (dataToUse)
{
}

void DataSynchronizer::applyChangeData (const void* data, size_t dataSize)
{
    serializing::fromBinary (data, dataSize, sData);
}

void DataSynchronizer::callback()
{
    juce::MemoryOutputStream m;
    sData.serialize().writeToStream (m);
    sendChangeData (m.getData(), m.getDataSize());
}

}  // namespace bav
