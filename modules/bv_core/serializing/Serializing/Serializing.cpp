namespace bav::serializing
{
void toBinary (SerializableData& data, juce::File file)
{
    juce::FileOutputStream stream (file);
    data.serialize().writeToStream (stream);
}

void toBinary (SerializableData& data, juce::MemoryBlock& dest)
{
    juce::MemoryOutputStream stream (dest, false);
    data.serialize().writeToStream (stream);
}

void fromBinary (juce::File file, SerializableData& dest)
{
    juce::MemoryBlock     data;
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
    dest.deserialize (newTree);
}

void fromXML (const juce::XmlElement& xml, SerializableData& dest)
{
    auto newTree = juce::ValueTree::fromXml (xml);
    dest.deserialize (newTree);
}

void fromXML (std::unique_ptr< juce::XmlElement > xml, SerializableData& dest)
{
    if (xml != nullptr)
        fromXML (*xml, dest);
}

void fromXML (const juce::File& xmlFile, SerializableData& dest)
{
    fromXML (juce::XmlDocument::parse (xmlFile), dest);
}

std::unique_ptr< juce::XmlElement > toXML (SerializableData& source)
{
    return source.serialize().createXml();
}

void toXML (SerializableData& source, const juce::File& file)
{
    auto xml = toXML (source);

    if (! file.existsAsFile())
        file.create();

    xml->writeTo (file);
}

void fromJSON (const juce::String& jsonText, SerializableData& dest)
{
    auto newTree = valueTreeFromJSON (jsonText);
    dest.deserialize (newTree);
}

juce::String toJSON (SerializableData& source)
{
    return valueTreeToJSON (source.serialize());
}

}  // namespace bav::serializing
