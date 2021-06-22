#pragma once

namespace bav::serializing
{
void toBinary (SerializableData& data, juce::File file);
void toBinary (SerializableData& data, juce::MemoryBlock& dest);

void fromBinary (juce::File file, SerializableData& dest);
void fromBinary (juce::MemoryBlock& data, SerializableData& dest);
void fromBinary (const void* data, size_t dataSizeInBytes, SerializableData& dest);

template < typename IntegerType >
void fromBinary (const void* data, IntegerType dataSizeInBytes, SerializableData& dest)
{
    fromBinary (data, static_cast< size_t > (dataSizeInBytes), dest);
}

std::unique_ptr< juce::XmlElement > toXML (SerializableData& source);
void                                toXML (SerializableData& source, const juce::File& file);

void fromXML (const juce::XmlElement& xml, SerializableData& dest);
void fromXML (std::unique_ptr< juce::XmlElement > xml, SerializableData& dest);
void fromXML (const juce::File& xmlFile, SerializableData& dest);

String toJSON (SerializableData& source);
void   fromJSON (const String& jsonText, SerializableData& dest);

}  // namespace bav::serializing
