#pragma once

#include "containers/Juce_containers.h"
#include "containers/STL_containers.h"

namespace lemons::serializing::TreeReflectorHelpers
{
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
String makePropertyNameForElement (const String& propertyName, int index);


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


/*-------------------------------- MAPS --------------------------------*/

/** Adds a key-pair map to the given TreeReflector. Used for both loading and saving. */
template < class MapType >
void addMap (TreeReflector& ref, MapType& map, const String& propertyName)
{
}

}  // namespace lemons::serializing::TreeReflectorHelpers
