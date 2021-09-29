#pragma once

#include "ValueTreeToJson/valuetree_json_converter.h"

#include "Specializations/AudioBuffers.h"
#include "Specializations/VarSpecializations.h"

namespace lemons::serializing
{
ValueTree toTree (SerializableData& data);
void      fromTree (const ValueTree& tree, SerializableData& data);

void copy (SerializableData& dest, SerializableData& source);

/*---------------------------------------------------------------------*/

void toBinary (SerializableData& data, File file);
void toBinary (SerializableData& data, juce::MemoryBlock& dest);

void fromBinary (File file, SerializableData& dest);
void fromBinary (const juce::MemoryBlock& data, SerializableData& dest);
void fromBinary (const void* data, size_t dataSizeInBytes, SerializableData& dest);

template < typename IntegerType >
void fromBinary (const void* data, IntegerType dataSizeInBytes, SerializableData& dest)
{
    fromBinary (data, static_cast< size_t > (dataSizeInBytes), dest);
}

template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromBinaryCreate (File file)
{
    ObjectType newObject;
    fromBinary (file, newObject);
    return newObject;
}

template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromBinaryCreate (const juce::MemoryBlock& data)
{
    ObjectType newObject;
    fromBinary (data, newObject);
    return newObject;
}

template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromBinaryCreate (const void* data, size_t dataSizeInBytes)
{
    ObjectType newObject;
    fromBinary (data, dataSizeInBytes, newObject);
    return newObject;
}

template < typename ObjectType, typename IntegerType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromBinaryCreate (const void* data, IntegerType dataSizeInBytes)
{
    return fromBinaryCreate< ObjectType > (data, static_cast< size_t > (dataSizeInBytes));
}

/*---------------------------------------------------------------------*/

std::unique_ptr< juce::XmlElement > toXML (SerializableData& source);
void                                toXML (SerializableData& source, const File& file);

void fromXML (const juce::XmlElement& xml, SerializableData& dest);
void fromXML (std::unique_ptr< juce::XmlElement > xml, SerializableData& dest);
void fromXML (const File& xmlFile, SerializableData& dest);

template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromXMLCreate (const juce::XmlElement& xml)
{
    ObjectType newObject;
    fromXML (xml, newObject);
    return newObject;
}

template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromXMLCreate (std::unique_ptr< juce::XmlElement > xml)
{
    ObjectType newObject;
    fromXML (xml, newObject);
    return newObject;
}

template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromXMLCreate (const File& xmlFile)
{
    ObjectType newObject;
    fromXML (xmlFile, newObject);
    return newObject;
}

/*---------------------------------------------------------------------*/

String toJSON (SerializableData& source);
void   toJSON (SerializableData& source, const File& file);

void fromJSON (const String& jsonText, SerializableData& dest);
void fromJSON (const File& file, SerializableData& dest);

template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromJSONCreate (const String& jsonText)
{
    ObjectType newObject;
    fromJSON (jsonText, newObject);
    return newObject;
}

template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromJSONCreate (const File& file)
{
    ObjectType newObject;
    fromJSON (file, newObject);
    return newObject;
}

}  // namespace lemons::serializing
