
namespace bav
{
TreeReflector::TreeReflector (const ValueTree& treeToUse)
    : tree (treeToUse)
{
    jassert (tree.isValid());
}

bool TreeReflector::isSaving() const { return ! isLoading(); }

void TreeReflector::loadDataChild (const String& propertyName, SerializableData& data) const
{
    data.restoreFromTree (tree.getChildWithName (propertyName));
}

void TreeReflector::saveDataChild (const String& propertyName, SerializableData& data)
{
    tree.appendChild (data.saveToTree (propertyName), nullptr);
}

void TreeReflector::loadValueTree (const String& propertyName, ValueTree& data) const
{
    const auto child = tree.getChildWithName (propertyName);

    if (! child.isValid()) return;

    data.copyPropertiesAndChildrenFrom (child, nullptr);
}

void TreeReflector::saveValueTree (const String& propertyName, ValueTree& data)
{
    ValueTree child {propertyName};

    child.copyPropertiesAndChildrenFrom (data, nullptr);

    tree.appendChild (child, nullptr);
}

ValueTree& TreeReflector::getRawDataTree() { return tree; }

bool TreeLoader::isLoading() const { return true; }

bool TreeSaver::isLoading() const { return false; }

}  // namespace bav
