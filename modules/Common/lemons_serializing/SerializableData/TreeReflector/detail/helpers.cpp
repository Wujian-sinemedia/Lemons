
namespace lemons::TreeReflectorHelpers
{
String propertyNameToContainerName (const String& propertyName)
{
    return propertyName + "s";
}

String makePropertyNameForElement (const String& propertyName, int& index)
{
    return propertyName + "_" + String (index++);
}

int getNumElementsOfType (const String& propertyName, const ValueTree& tree)
{
    int index = 0;

    do
    {
        const auto i = index;
        if (! tree.hasProperty (makePropertyNameForElement (propertyName, index)))
            return i;
    } while (index < tree.getNumProperties());

    return index;
}

void loadValueTree (const ValueTree& tree, const String& propertyName, ValueTree& data)
{
    const auto child = tree.getChildWithName (propertyName);

    if (! child.isValid()) return;

    data.copyPropertiesAndChildrenFrom (child, nullptr);
}

void saveValueTree (ValueTree& tree, const String& propertyName, ValueTree& data)
{
    ValueTree child {propertyName};

    child.copyPropertiesAndChildrenFrom (data, nullptr);

    tree.appendChild (child, nullptr);
}

}  // namespace lemons::TreeReflectorHelpers
