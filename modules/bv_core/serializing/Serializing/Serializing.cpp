namespace bav::serializing
{
void toBinary (SerializableData& data, juce::File file, bool isPreset)
{
    juce::FileOutputStream stream (file);
    data.serialize (isPreset).writeToStream (stream);
}

void toBinary (SerializableData& data, juce::MemoryBlock& dest, bool isPreset)
{
    juce::MemoryOutputStream stream (dest, false);
    data.serialize (isPreset).writeToStream (stream);
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
    dest.deserialize (ValueTree::readFromData (data, dataSizeInBytes));
}

void fromXML (const juce::XmlElement& xml, SerializableData& dest)
{
    dest.deserialize (ValueTree::fromXml (xml));
}

void fromXML (std::unique_ptr< juce::XmlElement > xml, SerializableData& dest)
{
    if (auto* x = xml.get())
        fromXML (*x, dest);
}

void fromXML (const juce::File& xmlFile, SerializableData& dest)
{
    fromXML (juce::XmlDocument::parse (xmlFile), dest);
}

std::unique_ptr< juce::XmlElement > toXML (SerializableData& source, bool isPreset)
{
    return source.serialize (isPreset).createXml();
}

void toXML (SerializableData& source, const juce::File& file, bool isPreset)
{
    if (! file.existsAsFile())
        file.create();

    toXML (source, isPreset)->writeTo (file);
}

void fromJSON (const String& jsonText, SerializableData& dest)
{
    dest.deserialize (valueTreeFromJSON (jsonText));
}

String toJSON (SerializableData& source, bool isPreset)
{
    return valueTreeToJSON (source.serialize (isPreset));
}

}  // namespace bav::serializing
