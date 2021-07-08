#pragma once

namespace bav::TreeReflectorHelpers
{
String propertyNameToContainerName (const String& propertyName);

template < typename Type, std::enable_if_t< std::is_enum_v< Type > > = nullptr >
Type toEnum (const juce::var& var)
{
    return static_cast< Type > (static_cast< std::underlying_type_t< Type > > ((int) var));
}

template < typename Type, std::enable_if_t< std::is_enum_v< Type > > = nullptr >
juce::var fromEnum (Type value)
{
    return static_cast< int > (static_cast< std::underlying_type_t< Type > > (value));
}


String makePropertyNameForElement (const String& propertyName, int& index);

int getNumElementsOfType (const String& propertyName, const ValueTree& tree);

template < class ContainerType >
void resizeContainer (ContainerType& container, int newSize)
{
    container.clear();

    if constexpr (is_specialization< ContainerType, std::vector >())
    {
        container.resize (static_cast< typename ContainerType::size_type > (newSize));
    }
    else
    {
        container.resize (newSize);
    }
}

template < class ContainerType >
void addContainer (TreeReflector& ref, ContainerType& container, const String& propertyName)
{
    if (ref.isLoading())
    {
        resizeContainer (container,
                         getNumElementsOfType (propertyName, ref.getRawDataTree()));
    }
    
    int index = 0;
    
    for (auto& element : container)
        ref.add (makePropertyNameForElement (propertyName, index),
                 element);
}

}  // namespace bav::TreeReflectorHelpers
