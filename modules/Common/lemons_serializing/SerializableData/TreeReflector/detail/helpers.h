#pragma once

#include "containers/Juce_containers.h"
#include "containers/STL_containers.h"

namespace lemons::serializing::TreeReflectorHelpers
{
/** Returns true if the given object inherits from SerializableData and is not const.
    @see SerializableData
 */
template < typename Type >
constexpr bool isSerializable()
{
    return std::is_base_of< SerializableData, Type >() && ! std::is_const< Type >();
}


/*--------------------------------  OBJECTS -------------------------------*/

/** Converts a juce::var to a specified enum type. */
template < typename Type >
Type toEnum (const juce::var& var)
{
    return static_cast< Type > (static_cast< std::underlying_type_t< Type > > ((int) var));
}


/** Converts an enum type to a juce::var. */
template < typename Type >
juce::var fromEnum (Type value)
{
    return static_cast< int > (static_cast< std::underlying_type_t< Type > > (value));
}

/** Restores the state of an object by retrieving the specified named property from the ValueTree, if it exists.
    @see serializing::fromVar(), toEnum()
 */
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

/** Saves the state of an object by writing its juce::var representation to the ValueTree with the specified property name.
    @see serializing::toVar(), fromEnum()
 */
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

/** Restores the state of a child ValueTree with the specified name. */
void loadValueTree (const ValueTree& tree, const String& propertyName, ValueTree& data);

/** Saves the state of a child ValueTree to a sub-node in the parent tree with the specified name. */
void saveValueTree (ValueTree& tree, const String& propertyName, ValueTree& data);


/*-------------------------------- CONTAINERS -----------------------------*/

/** Transforms the name of a  single container element to a name for the container's sub-node.
    This simply appends an "s" to the passed propertyName. \n
    For example, passing "Item" will return "Items".
 */
String propertyNameToContainerName (const String& propertyName);


/** Transforms a passed generic container element name and container index into a unique name for a given object.
    The transformation is:
    @verbatim
    {propertyName}_{index}
    @endverbatim
    For example, passing "Item" with an index of 3 will return "Item_3".
 */
String makePropertyNameForElement (const String& propertyName, int& index);


/** Detects how many elements a given container contains.
    Uses the same transformation as makePropertyNameForElement() and counts how many valid indices it can find corresponding to existing property names.
    @see makePropertyNameForElement()
 */
int getNumElementsOfType (const String& propertyName, const ValueTree& tree);


/** Adds a container to the given TreeReflector. Used for both loading and saving. */
template < class ContainerType >
void addContainer (TreeReflector& ref, ContainerType& container, const String& propertyName)
{
    if (ref.isLoading())
    {
        serializing::getInterfaceForContainer (container)->resize (getNumElementsOfType (propertyName,
                                                                                         ref.getRawDataTree()));
    }

    int index = 1;

    for (auto& element : container)
    {
        ref.add (makePropertyNameForElement (propertyName, index++),
                 element);
    }
}

}  // namespace lemons::serializing::TreeReflectorHelpers
