#pragma once

namespace bav::TreeReflectorHelpers
{

String makePropertyNameForElement (const String& propertyName, int& index);

int getNumElementsOfType (const String& propertyName, const ValueTree& tree);

template<class ContainerType>
void resizeContainer (ContainerType& container, const String& propertyName, const ValueTree& tree)
{
    const auto newSize = TreeReflectorHelpers::getNumElementsOfType (propertyName, tree);
    
    container.clear();
    
    if constexpr (is_specialization<ContainerType, std::vector>())
    {
        container.resize (static_cast<typename ContainerType::size_type> (newSize));
    }
    else
    {
        container.resize (newSize);
    }
}

}
