
namespace lemons
{
TreeReflector::TreeReflector (const ValueTree& treeToUse)
    : tree (treeToUse)
{
    jassert (tree.isValid());
}

bool TreeReflector::isSaving() const { return ! isLoading(); }

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

void TreeReflector::loadDataChild (const String& propertyName, SerializableData& data) const
{
    data.restoreFromTree (tree.getChildWithName (propertyName));
}

void TreeReflector::saveDataChild (const String& propertyName, SerializableData& data)
{
    tree.appendChild (data.saveToTree (propertyName), nullptr);
}

ValueTree& TreeReflector::getRawDataTree() { return tree; }

/*-------------------------------- Misc helper funcs ------------------------------*/

String TreeReflector::propertyNameToContainerName (const String& propertyName) const
{
    return propertyName + "s";
}

String TreeReflector::makePropertyNameForElement (const String& propertyName, int index) const
{
    return propertyName + "_" + String (index);
}

int TreeReflector::getNumElementsOfType (const String& propertyName, const ValueTree& tree) const
{
    const auto num = tree.getNumProperties();

    juce::Array< String > names;

    for (int i = 1; i <= num; ++i)
    {
        names.add (makePropertyNameForElement (propertyName, i));
    }

    auto actualNum = num;

    for (int i = 0; i < num; ++i)
    {
        if (! names.contains (tree.getPropertyName (i).toString()))
        {
            --actualNum;
        }
    }

    return actualNum;
}

/*------------------------------------------------------------------------------------------------------*/

bool TreeLoader::isLoading() const { return true; }

bool TreeSaver::isLoading() const { return false; }

}  // namespace lemons
