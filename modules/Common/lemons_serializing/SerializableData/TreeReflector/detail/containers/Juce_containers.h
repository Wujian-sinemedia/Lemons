#pragma once

namespace lemons::serializing
{
template < typename ElementType >
struct JuceArrayInterface : LambdaContainerInterface< juce::Array< ElementType > >
{
    using Container = juce::Array< ElementType >;

    JuceArrayInterface (Container& container)
        : LambdaContainerInterface< Container > (container, [] (Container& c, int newSize)
                                                 { c.resize (newSize); })
    {
    }
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
struct JuceOwnedArrayInterface : LambdaContainerInterface< juce::OwnedArray< ElementType > >
{
    using Container = juce::OwnedArray< ElementType >;

    JuceOwnedArrayInterface (Container& container)
        : LambdaContainerInterface< Container > (container, [] (Container& c, int newSize)
                                                 {
                                                     const auto prevSize = c.size();

                                                     for (int i = newSize; i < prevSize; ++i)
                                                         c.remove (i, true);

                                                     for (int i = prevSize; i < newSize; ++i)
                                                         c.add (new ElementType());
                                                 })
    {
    }
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

}  // namespace lemons::serializing
