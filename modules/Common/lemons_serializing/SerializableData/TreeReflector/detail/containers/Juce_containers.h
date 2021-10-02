#pragma once

namespace lemons::serializing
{
/** Container interface that provides support for serializing juce::Arrays with TreeReflector. */
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


/** Returns an instance of JuceArrayInterface for the given juce::Array. */
template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (juce::Array< ElementType >& container)
{
    return std::make_unique< JuceArrayInterface< ElementType > > (container);
}


/** Specifies that any specialization of juce::Array is a serializable container. */
template < typename ElementType >
struct isContainer< juce::Array< ElementType > > : std::true_type
{
};


/*------------------------------------------------------------------------------------*/


/** Container interface that provides support for serializing juce::OwnedArrays with TreeReflector. */
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


/** Returns an instance of JuceOwnedArrayInterface for the given juce::OwnedArray. */
template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (juce::OwnedArray< ElementType >& container)
{
    return std::make_unique< JuceOwnedArrayInterface< ElementType > > (container);
}


/** Specifies that any specialization of juce::OwnedArray is a serializable container. */
template < typename ElementType >
struct isContainer< juce::OwnedArray< ElementType > > : std::true_type
{
};

}  // namespace lemons::serializing
