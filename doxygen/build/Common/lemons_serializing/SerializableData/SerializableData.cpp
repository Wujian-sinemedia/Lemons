
/** @ingroup SerializableData
 *  @{
 */

namespace lemons
{
SerializableData::SerializableData (const juce::String& identifier)
    : dataIdentifier (identifier)
{
}

[[nodiscard]] ValueTree SerializableData::serialize()
{
    ValueTree tree {dataIdentifier};

    TreeSaver ref {tree};

    serialize (ref);

    return tree;
}

void SerializableData::deserialize (const ValueTree& t)
{
    const auto tree = t.hasType (dataIdentifier) ? t : t.getChildWithName (dataIdentifier);

    if (! tree.isValid()) return;

    TreeLoader ref {tree};

    serialize (ref);
}

}  // namespace lemons

/** @}*/
