#pragma once

namespace lemons::serializing::TreeReflectorHelpers
{
template < typename ElementType >
struct JuceArrayInterface : ContainerInterface
{
    using Type = juce::Array< ElementType >;

    JuceArrayInterface (Type& containerToUse)
        : container (containerToUse)
    {
    }

private:
    void resize (int newSize) final
    {
        container.resize (newSize);
    }

    Type& container;
};

template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (juce::Array< ElementType >& container)
{
    return std::make_unique< JuceArrayInterface< ElementType > > (container);
}


template < typename ElementType >
struct isContainer< juce::Array< ElementType > > : std::true_type
{
};


/*------------------------------------------------------------------------------------*/


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
        const auto prevSize = container.size();

        for (int i = newSize; i < prevSize; ++i)
            container.remove (i, true);

        for (int i = prevSize; i < newSize; ++i)
            container.add (new ElementType());
    }

    Type& container;
};

template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (juce::OwnedArray< ElementType >& container)
{
    return std::make_unique< JuceOwnedArrayInterface< ElementType > > (container);
}

template < typename ElementType >
struct isContainer< juce::OwnedArray< ElementType > > : std::true_type
{
};

}  // namespace lemons::serializing::TreeReflectorHelpers
