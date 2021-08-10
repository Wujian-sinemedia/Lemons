#pragma once

namespace bav::TreeReflectorHelpers
{
template < typename ElementType >
struct JuceArrayInterface : ContainerInterface
{
    using ArrayType = juce::Array< ElementType >;

    JuceArrayInterface (ArrayType& containerToUse)
        : container (containerToUse)
    {
    }

private:
    void resize (int newSize) final
    {
        container.ensureStorageAllocated (newSize);
    }

    ArrayType& container;
};

template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (juce::Array< ElementType >& container)
{
    return std::make_unique< JuceArrayInterface< ElementType > > (container);
}

}  // namespace bav::TreeReflectorHelpers
