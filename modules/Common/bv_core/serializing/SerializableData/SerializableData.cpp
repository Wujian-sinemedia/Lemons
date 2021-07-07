
namespace bav
{
TreeReflector::TreeReflector (ValueTree treeToUse, bool loading)
    : tree (treeToUse), loadingData (loading)
{
}

bool TreeReflector::isLoading() const
{
    return loadingData;
}

void TreeReflector::saveDataChild (const String& propertyName, SerializableData& data)
{
    tree.appendChild (data.serialize (juce::Identifier (propertyName)), nullptr);
}

void TreeReflector::loadDataChild (const String& propertyName, SerializableData& data)
{
    const auto child = tree.getChildWithName (juce::Identifier (propertyName));

    if (child.isValid())
        data.deserialize (child);
}

ValueTree& TreeReflector::getRawDataTree()
{
    return tree;
}


SerializableData::SerializableData (juce::Identifier identifier)
    : dataIdentifier (identifier)
{
}


SerializableData& SerializableData::operator= (SerializableData& other)
{
    deserialize (other.serialize());
}

ValueTree SerializableData::serialize (juce::Identifier treeID)
{
    ValueTree tree {treeID};

    TreeReflector ref {tree, false};
    serialize (ref);

    return tree;
}


void SerializableData::deserialize (const ValueTree& t)
{
    if (t.hasType (dataIdentifier))
        setTree (t);
    else
        setTree (t.getChildWithName (dataIdentifier));
}

void SerializableData::setTree (const ValueTree& newTree)
{
    if (! newTree.isValid()) return;

    TreeReflector ref {newTree, true};
    serialize (ref);
}

}  // namespace bav
