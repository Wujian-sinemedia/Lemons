#pragma once

#include "containers/ContainerInterface.h"

namespace bav::TreeReflectorHelpers
{
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

}  // namespace bav::TreeReflectorHelpers
