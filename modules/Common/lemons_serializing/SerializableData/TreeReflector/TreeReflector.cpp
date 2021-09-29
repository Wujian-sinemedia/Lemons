
namespace lemons
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

ValueTree& TreeReflector::getRawDataTree() { return tree; }

bool TreeLoader::isLoading() const { return true; }

bool TreeSaver::isLoading() const { return false; }

}  // namespace lemons
