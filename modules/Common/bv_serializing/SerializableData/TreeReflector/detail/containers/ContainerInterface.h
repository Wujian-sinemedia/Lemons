#pragma once

namespace bav::TreeReflectorHelpers
{
struct ContainerInterface
{
    virtual ~ContainerInterface() = default;

    virtual void resize (int newSize) = 0;
};

template < typename ContainerType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (ContainerType&);

template < class Type >
constexpr bool isContainer()
{
    return std::is_array< Type >() || is_specialization< Type, std::vector >() || is_specialization< Type, juce::OwnedArray >();
}

}  // namespace bav::TreeReflectorHelpers

#include "STL_containers.h"
#include "Juce_containers.h"
