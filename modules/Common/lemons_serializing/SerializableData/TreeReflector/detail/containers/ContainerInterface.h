#pragma once

namespace lemons::TreeReflectorHelpers
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

}  // namespace lemons::TreeReflectorHelpers

#define BV_TRF_DECLARE_CONTAINER_INTERFACE(ContainerClass, InterfaceClassName, operationToResize)             \
    template < typename ElementType >                                                                         \
    struct InterfaceClassName : ContainerInterface                                                            \
    {                                                                                                         \
        using Type = ContainerClass< ElementType >;                                                           \
                                                                                                              \
        InterfaceClassName (Type& containerToUse)                                                             \
            : container (containerToUse)                                                                      \
        {                                                                                                     \
        }                                                                                                     \
                                                                                                              \
    private:                                                                                                  \
        void resize (int newSize) final                                                                       \
        {                                                                                                     \
            operationToResize;                                                                                \
        }                                                                                                     \
                                                                                                              \
        Type& container;                                                                                      \
    };                                                                                                        \
                                                                                                              \
    template < typename ElementType >                                                                         \
    std::unique_ptr< ContainerInterface > getInterfaceForContainer (ContainerClass< ElementType >& container) \
    {                                                                                                         \
        return std::make_unique< InterfaceClassName< ElementType > > (container);                             \
    }

#include "STL_containers.h"
#include "Juce_containers.h"

#undef BV_TRF_DECLARE_CONTAINER_INTERFACE
