
namespace lemons
{
SerializableData::SerializableData (const juce::String& identifier)
    : dataIdentifier (identifier.toStdString())
{
}

ValueTree SerializableData::serialize()
{
    ValueTree tree {juce::Identifier (dataIdentifier)};

    TreeSaver ref {tree};

    serialize (ref);

    return tree;
}

void SerializableData::deserialize (const ValueTree& t)
{
    juce::Identifier id {dataIdentifier};
    const auto       tree = t.hasType (id) ? t : t.getChildWithName (id);

    if (! tree.isValid()) return;

    TreeLoader ref {tree};

    serialize (ref);
}

}  // namespace lemons
