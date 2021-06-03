
namespace bav
{
DataSynchronizer::DataSynchronizer (SerializableData& dataToUse)
    : sData (dataToUse)
{
    Timer::startTimerHz (10);
}

DataSynchronizer::~DataSynchronizer()
{
    Timer::stopTimer();
}


void DataSynchronizer::applyChangeData (const void* data, size_t dataSize)
{
    serializing::fromBinary (data, dataSize, sData);
}


void DataSynchronizer::timerCallback()
{
    juce::MemoryOutputStream m;
    sData.serialize().writeToStream (m);
    sendChangeData (m.getData(), m.getDataSize());
}

}  // namespace bav
