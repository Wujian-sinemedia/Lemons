#pragma once

namespace lemons::serializing::TreeReflectorHelpers
{
struct ContainerInterface
{
    virtual ~ContainerInterface() = default;

    virtual void resize (int newSize) = 0;
};

template < typename ContainerType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (ContainerType&);

template < typename Type >
struct isContainer : std::false_type
{
};

}  // namespace lemons::serializing::TreeReflectorHelpers


#include "STL_containers.h"
#include "Juce_containers.h"
