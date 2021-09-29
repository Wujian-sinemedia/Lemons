#pragma once

namespace lemons::TreeReflectorHelpers
{
BV_TRF_DECLARE_CONTAINER_INTERFACE (juce::Array, JuceArrayInterface, container.ensureStorageAllocated (newSize))


template < typename ElementType >
struct JuceOwnedArrayInterface : ContainerInterface
{
    using Type = juce::OwnedArray< ElementType >;

    JuceOwnedArrayInterface (Type& containerToUse)
        : container (containerToUse)
    {
    }

private:
    void resize (int newSize) final
    {
        container.clear();

        for (int i = 0; i < newSize; ++i)
            container.add (new ElementType());
    }

    Type& container;
};

template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (juce::OwnedArray< ElementType >& container)
{
    return std::make_unique< JuceOwnedArrayInterface< ElementType > > (container);
}

}  // namespace lemons::TreeReflectorHelpers
