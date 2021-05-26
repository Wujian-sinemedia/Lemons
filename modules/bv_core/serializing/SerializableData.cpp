
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


/*-----------------------------------------------------------------------------------------------------------------------
 -----------------------------------------------------------------------------------------------------------------------*/

void toBinary (SerializableData& data, juce::File file)
{
    juce::FileOutputStream stream (file);
    data.serialize().writeToStream (stream);
}

void toBinary (SerializableData& data, juce::File& file)
{
    juce::FileOutputStream stream (file);
    data.serialize().writeToStream (stream);
}

void toBinary (SerializableData& data, juce::MemoryBlock& dest)
{
    juce::MemoryOutputStream stream (dest, false);
    data.serialize().writeToStream (stream);
}

void fromBinary (juce::File& file, SerializableData& dest)
{
    juce::MemoryBlock data;
    juce::FileInputStream stream (file);
    stream.readIntoMemoryBlock (data);
    fromBinary (data.getData(), data.getSize(), dest);
}

void fromBinary (juce::MemoryBlock& data, SerializableData& dest)
{
    fromBinary (data.getData(), data.getSize(), dest);
}

void fromBinary (const void* data, size_t dataSizeInBytes, SerializableData& dest)
{
    auto newTree = juce::ValueTree::readFromData (data, dataSizeInBytes);
    jassert (newTree.isValid());
    dest.deserialize (newTree);
}

void fromBinary (const void* data, int dataSizeInBytes, SerializableData& dest)
{
    fromBinary (data, static_cast<size_t> (dataSizeInBytes), dest);
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
    fromBinary (data, dataSize, sData);
}


void DataSynchronizer::timerCallback()
{
    juce::MemoryOutputStream m;
    sData.serialize().writeToStream (m);
    sendChangeData (m.getData(), m.getDataSize());
}
    

}  // namespace
