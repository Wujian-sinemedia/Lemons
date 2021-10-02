#pragma once

#include "ValueTreeToJson/valuetree_json_converter.h"

#include "Specializations/AudioBuffers.h"
#include "Specializations/VarSpecializations.h"

namespace lemons::serializing
{
/** Copies from one serializable object to another by serializing the source object to a ValueTree, then deserializing the dest object from that tree. */
void copy (SerializableData& dest, SerializableData& source);

/*---------------------------------------------------------------------*/

/** Serializes an object to a ValueTree. */
ValueTree toTree (SerializableData& data);

/** Deserializes an object from a ValueTree. */
void fromTree (const ValueTree& tree, SerializableData& data);

/** Creates a new object from the state saved in the ValueTree.
    @tparam ObjectType The type of object to create. This type must inherit from SerializableData and must be default-constructable.
 */
template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromTreeCreate (const ValueTree& tree)
{
    ObjectType newObject;
    fromTree (tree, newObject);
    return newObject;
}

/*---------------------------------------------------------------------*/

/** Saves the state of an object to a file.
    Internally, this method calls juce::ValueTree::writeToStream(), so the data is stored as an arbitrary binary encoding of a ValueTree, not necessarily a common format like XML or JSON. \n \n
    Use the XML- and JSON-specific functions in this namespace to explicitly use either format.
    @see toXML(), toJSON()
 */
void toBinary (SerializableData& data, File file);

/** Saves the state of an object to a juce::MemoryBlock. */
void toBinary (SerializableData& data, juce::MemoryBlock& dest);

/** Deserializes an object from a file. */
void fromBinary (File file, SerializableData& dest);

/** Deserializes an object from a juce::MemoryBlock. */
void fromBinary (const juce::MemoryBlock& data, SerializableData& dest);

/** Deserializes an object from some arbitrary opaque binary data. */
void fromBinary (const void* data, size_t dataSizeInBytes, SerializableData& dest);

/** Deserializes an object from some arbitrary opaque binary data. */
template < typename IntegerType >
void fromBinary (const void* data, IntegerType dataSizeInBytes, SerializableData& dest)
{
    fromBinary (data, static_cast< size_t > (dataSizeInBytes), dest);
}

/** Creates a new object from the state saved in the file.
    @tparam ObjectType The type of object to create. This type must inherit from SerializableData and must be default-constructable.
 */
template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromBinaryCreate (File file)
{
    ObjectType newObject;
    fromBinary (file, newObject);
    return newObject;
}

/** Creates a new object from the state saved in the MemoryBlock.
    @tparam ObjectType The type of object to create. This type must inherit from SerializableData and must be default-constructable.
 */
template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromBinaryCreate (const juce::MemoryBlock& data)
{
    ObjectType newObject;
    fromBinary (data, newObject);
    return newObject;
}

/** Creates a new object from the state saved in the opaque binary data.
    @tparam ObjectType The type of object to create. This type must inherit from SerializableData and must be default-constructable.
 */
template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromBinaryCreate (const void* data, size_t dataSizeInBytes)
{
    ObjectType newObject;
    fromBinary (data, dataSizeInBytes, newObject);
    return newObject;
}

/** Creates a new object from the state saved in the opaque binary data.
    @tparam ObjectType The type of object to create. This type must inherit from SerializableData and must be default-constructable.
 */
template < typename ObjectType, typename IntegerType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromBinaryCreate (const void* data, IntegerType dataSizeInBytes)
{
    return fromBinaryCreate< ObjectType > (data, static_cast< size_t > (dataSizeInBytes));
}

/*---------------------------------------------------------------------*/

/** Serializes an object to an XML element. */
std::unique_ptr< juce::XmlElement > toXML (SerializableData& source);

/** Serializes an object to an XML file. */
void toXML (SerializableData& source, const File& file);

/** Deserializes an object from an XML element. */
void fromXML (const juce::XmlElement& xml, SerializableData& dest);

/** Deserializes an object from an XML element. */
void fromXML (std::unique_ptr< juce::XmlElement > xml, SerializableData& dest);

/** Deserializes an object from an XML file. */
void fromXML (const File& xmlFile, SerializableData& dest);

/** Creates a new object from the state saved in the XML element.
    @tparam ObjectType The type of object to create. This type must inherit from SerializableData and must be default-constructable.
 */
template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromXMLCreate (const juce::XmlElement& xml)
{
    ObjectType newObject;
    fromXML (xml, newObject);
    return newObject;
}

/** Creates a new object from the state saved in the XML element.
    @tparam ObjectType The type of object to create. This type must inherit from SerializableData and must be default-constructable.
 */
template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromXMLCreate (std::unique_ptr< juce::XmlElement > xml)
{
    ObjectType newObject;
    fromXML (xml, newObject);
    return newObject;
}

/** Creates a new object from the state saved in the XML file.
    @tparam ObjectType The type of object to create. This type must inherit from SerializableData and must be default-constructable.
 */
template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromXMLCreate (const File& xmlFile)
{
    ObjectType newObject;
    fromXML (xmlFile, newObject);
    return newObject;
}

/*---------------------------------------------------------------------*/

/** Serializes an object to a JSON string. */
String toJSON (SerializableData& source);

/** Serializes an object to a JSON file. */
void toJSON (SerializableData& source, const File& file);

/** Deserializes an object from a JSON string. */
void fromJSON (const String& jsonText, SerializableData& dest);

/** Deserializes an object from a JSON file. */
void fromJSON (const File& file, SerializableData& dest);

/** Creates a new object from the state saved in the JSON string.
    @tparam ObjectType The type of object to create. This type must inherit from SerializableData and must be default-constructable.
    @see valueTreeFromJSON()
 */
template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromJSONCreate (const String& jsonText)
{
    ObjectType newObject;
    fromJSON (jsonText, newObject);
    return newObject;
}

/** Creates a new object from the state saved in the JSON file.
    @tparam ObjectType The type of object to create. This type must inherit from SerializableData and must be default-constructable.
    @see valueTreeToJSON()
 */
template < typename ObjectType, BV_MUST_INHERIT_FROM (ObjectType, SerializableData) >
ObjectType fromJSONCreate (const File& file)
{
    ObjectType newObject;
    fromJSON (file, newObject);
    return newObject;
}

}  // namespace lemons::serializing
