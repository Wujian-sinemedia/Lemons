
namespace lemons::serializing::TreeReflectorHelpers
{
String propertyNameToContainerName (const String& propertyName)
{
    return propertyName + "s";
}

String makePropertyNameForElement (const String& propertyName, int index)
{
    return propertyName + "_" + String (index);
}

int getNumElementsOfType (const String& propertyName, const ValueTree& tree)
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

}  // namespace lemons::serializing::TreeReflectorHelpers
