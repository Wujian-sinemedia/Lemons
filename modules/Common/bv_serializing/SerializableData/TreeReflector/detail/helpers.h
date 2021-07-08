#pragma once

namespace bav::TreeReflectorHelpers
{
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
void resizeContainer (ContainerType& container, const String& propertyName, const ValueTree& tree)
{
    const auto newSize = TreeReflectorHelpers::getNumElementsOfType (propertyName, tree);

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

}  // namespace bav::TreeReflectorHelpers
