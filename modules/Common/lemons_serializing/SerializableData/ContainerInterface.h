#pragma once

#include <functional>

namespace lemons::serializing
{
/**
    Interface that defines how a TreeReflector can resize a given container during deserialization. \n
    Implement this if you want to be able to serialize a custom container type with a single TreeReflector::add() call. \n
    \n
    @snippet[doc] this Adding a custom container
    For a slicker interface that allows you to supply a lambda, see LambdaContainerInterface.
    @see LambdaContainerInterface, TreeReflector, getInterfaceForContainer(), isContainer
 */
struct ContainerInterface
{
    virtual ~ContainerInterface() = default;

    /** Your subclass must implement this to change the total number of items in the container.
        After TreeReflector calls this function, it will use a range-based for loop to traverse your container, so its size needs to be correct, and you shouldn't keep any "idle" objects around or reserved memory allocated after this function call.
     */
    virtual void resize (int newSize) = 0;
};


/** Provides a slightly slicker interface to ContainerInterface, allowing you to supply a lambda function to resize your container. \n
    Example usage:
    @code
    template<typename ElementType>
    struct JuceArrayInterface : LambdaContainerInterface<juce::Array<ElementType>>
    {
        using Container = juce::Array<ElementType>;

        JuceArrayInterface (Container& container)
            : LambdaContainerInterface<Container>(container, [](Container& c, int newSize)
                                                             {
                                                                 c.resize (newSize);
                                                             })
        { }
    };

    template < typename ElementType >
    std::unique_ptr< ContainerInterface > getInterfaceForContainer (juce::Array< ElementType >& container)
    {
        return std::make_unique< JuceArrayInterface< ElementType > > (container);
    }
    @endcode
    @tparam ContainerType The fully specialized container type -- for example, std::vector<int> or juce::Array<std::vector<float>>.
    @see ContainerInterface, TreeReflector, getInterfaceForContainer(), isContainer
 */
template < class ContainerType >
struct LambdaContainerInterface : ContainerInterface
{
    using ResizeCallback = std::function< void (ContainerType&, int) >;

    LambdaContainerInterface (ContainerType& containerToUse, ResizeCallback resizeFuncToUse)
        : resizeFunc (std::move (resizeFuncToUse)), container (containerToUse)
    {
    }

private:
    void resize (int newSize) final
    {
        resizeFunc (container, newSize);
    }

    ResizeCallback resizeFunc;
    ContainerType& container;
};


/**
    Template function that returns an instance of the correct ContainerInterface for the given container.
    Implement this if you want to be able to serialize a custom container type with a single TreeReflector::add() call. \n
    \n
    @snippet[doc] this Adding a custom container
    @tparam ContainerType The fully specialized container type -- for example, std::vector<int> or juce::Array<float>.
    @see TreeReflector, ContainerInterface, isContainer
 */
template < typename ContainerType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (ContainerType&);


/**
    Template struct that that indicates to TreeReflector whether a given type is a container.
    Implement this if you want to be able to serialize a custom container type with a single TreeReflector::add() call. \n
    \n
    @snippet[doc] this Adding a custom container
    @tparam Type The type being tested. For example, int or String will return false, but std::vector<int> will return true.
    @see TreeReflector, ContainerInterface, getInterfaceForContainer()
 */
template < typename Type >
struct isContainer : std::false_type
{
};

}  // namespace lemons::serializing


/**
    [Adding a custom container]
    To add support for a custom container type to TreeReflector::add():
    - Implement a subclass of serializing::ContainerInterface with the correct resizing logic for your cotainer;
    - Implement a specialization ofserializing::getContainerForInterface() that returns an instance of your custom interface;
    - Specialize the serializing::isContainer struct for your type and make the specialization inherit from std::true_type.

    In addition, your custom container type must have begin() and end() functions -- ie, it must be compatable with range-based for loops.

    The elements of your container must also be valid TreeReflector::add() calls -- ie, if they don't inherit SerializableData, you must implement toVar() and fromVar() for the container's element type as well.

    Here is an example implementation for a custom type:

    template<typename ElementType>
    struct MyContainerType
    {
        ElementType* begin();
        ElementType* end();

        void setSize (int);
    };

    // Step 1: implement container interface
    // This tells the TreeReflector how to resize your container

    template<typename ElementType>
    struct MyContainerInterface : lemons::serializing::ContainerInterface
    {
        using Container = MyContainerType<ElementType>;

        MyContainerInterface (Container& c) : container (c) { }

    private:
        void resize (int newSize) final
    {
        container.setSize (newSize);
    }

        Container& container;
    };

    // Step 2: implement getInterfaceForContainer()
    // This creates an instance of your container interface for the TreeReflector to use

    template < typename ElementType >
    std::unique_ptr< ContainerInterface > getInterfaceForContainer (MyContainerType< ElementType >& container)
    {
        return std::make_unique< MyContainerInterface< ElementType > > (container);
    }

    // Step 3: implement isContainer
    // This tells TreeReflector that any specialization of your container type is a kind of container

    template < typename ElementType >
    struct isContainer< MyContainerType< ElementType > > : std::true_type
    {
    };
    [Adding a custom container]
 */
