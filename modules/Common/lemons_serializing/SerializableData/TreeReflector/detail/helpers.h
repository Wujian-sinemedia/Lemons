#pragma once

#include "containers/Juce_containers.h"
#include "containers/STL_containers.h"

namespace lemons::serializing::TreeReflectorHelpers
{
template < typename Type >
constexpr bool isSerializable()
{
    return std::is_base_of< SerializableData, Type >() && ! std::is_const< Type >();
}

/*--------------------------------  OBJECTS -------------------------------*/

template < typename Type >
Type toEnum (const juce::var& var)
{
    return static_cast< Type > (static_cast< std::underlying_type_t< Type > > ((int) var));
}

template < typename Type >
juce::var fromEnum (Type value)
{
    return static_cast< int > (static_cast< std::underlying_type_t< Type > > (value));
}

template < typename Type >
void loadObject (const ValueTree& tree, const String& propertyName, Type& object)
{
    if (! tree.hasProperty (propertyName))
        return;

    const juce::var& var = tree.getProperty (propertyName);

    if constexpr (std::is_enum< Type >())
        object = toEnum< Type > (var);
    else
        object = serializing::fromVar< Type > (var);
}

template < typename Type >
void saveObject (ValueTree& tree, const String& propertyName, Type& object)
{
    juce::var var;

    if constexpr (std::is_enum< Type >())
        var = fromEnum (object);
    else
        var = serializing::toVar (object);

    tree.setProperty (propertyName, var, nullptr);
}

/*------------------------------- VALUE TREES -----------------------------*/

void loadValueTree (const ValueTree& tree, const String& propertyName, ValueTree& data);
void saveValueTree (ValueTree& tree, const String& propertyName, ValueTree& data);

/*-------------------------------- CONTAINERS -----------------------------*/

String propertyNameToContainerName (const String& propertyName);

String makePropertyNameForElement (const String& propertyName, int& index);

int getNumElementsOfType (const String& propertyName, const ValueTree& tree);

template < class ContainerType >
void addContainer (TreeReflector& ref, ContainerType& container, const String& propertyName)
{
    if (ref.isLoading())
    {
        auto interface = getInterfaceForContainer (container);

        interface->resize (getNumElementsOfType (propertyName, ref.getRawDataTree()));
    }

    int index = 0;

    for (auto& element : container)
    {
        ref.add (makePropertyNameForElement (propertyName, index),
                 element);
    }
}

}  // namespace lemons::serializing::TreeReflectorHelpers
