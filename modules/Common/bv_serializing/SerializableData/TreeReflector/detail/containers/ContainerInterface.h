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

}  // namespace bav::TreeReflectorHelpers

#include "STL_containers.h"
#include "Juce_containers.h"
