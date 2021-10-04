
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

int TreeReflector::getNumContainerElements (const String& propertyName) const
{
    const auto properties = tree.getNumProperties();
    const auto children   = tree.getNumChildren();
    const auto total      = properties + children;

    juce::Array< String > names;

    for (int i = 1; i <= total; ++i)
    {
        names.add (makePropertyNameForElement (propertyName, i));
    }

    auto actualNum = total;

    for (int i = 0; i < properties; ++i)
    {
        if (! names.contains (tree.getPropertyName (i).toString()))
        {
            --actualNum;
        }
    }

    for (int i = 0; i < children; ++i)
    {
        if (! names.contains (tree.getChild (i).getType().toString()))
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
