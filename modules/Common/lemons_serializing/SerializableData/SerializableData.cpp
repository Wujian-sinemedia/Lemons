
namespace lemons
{
SerializableData::SerializableData (juce::Identifier identifier)
    : dataIdentifier (identifier)
{
}

ValueTree SerializableData::serialize()
{
    ValueTree tree {dataIdentifier};

    TreeSaver ref {tree};

    serialize (ref);

    return tree;
}

void SerializableData::deserialize (const ValueTree& t)
{
    const auto tree = [&]() -> ValueTree
    {
        if (t.hasType (dataIdentifier))
            return t;
        else
            return t.getChildWithName (dataIdentifier);
    }();

    if (! tree.isValid()) return;

    TreeLoader ref {tree};

    serialize (ref);
}

}  // namespace lemons
