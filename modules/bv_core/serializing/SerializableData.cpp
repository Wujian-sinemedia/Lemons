
namespace bav
{

SerializableData::SerializableData (juce::Identifier identifier)
    : dataIdentifier (identifier)
{ }


juce::ValueTree SerializableData::serialize()
{
    ValueTree tree {dataIdentifier};
    toValueTree (tree);
    return tree;
}


juce::ValueTree& SerializableData::serialize (ValueTree& tree)
{
    ValueTree child {dataIdentifier};
    toValueTree (child);
    tree.appendChild (child, nullptr);
    return tree;
}


void SerializableData::deserialize (const ValueTree& tree)
{
    auto t = tree.getChildWithName (dataIdentifier);
    jassert (t.isValid());
    fromValueTree (t);
}


void SerializableData::deserialize (const void* data, int sizeInBytes)
{
    auto newTree = ValueTree::readFromData (data, static_cast< size_t > (sizeInBytes));
    if (! newTree.isValid()) return;
    
    deserialize (newTree);
}


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/


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
    sData.deserialize (data, dataSize);
}


void DataSynchronizer::timerCallback()
{
    juce::MemoryOutputStream m;
    sData.serialize().writeToStream (m);
    sendChangeData (m.getData(), m.getDataSize());
}
    

}  // namespace
