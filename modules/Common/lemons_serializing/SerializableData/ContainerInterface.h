#pragma once

namespace lemons::serializing
{
/**
    Interface that defines how a TreeReflector can resize a given container during deserialization.
    To add support for a custom container type to TreeReflector::add():
    - Implement a subclass of this interface with the correct resizing logic for your cotainer;
    - Implement the getInterfaceForContainer() function, specialized for your container type, returning an instance of the container interface for your custom type;
    - Specialize the isContainer struct for your type and make the specialization inherit from std::true_type.
 
    In addition, your custom container type must have begin() and end() functions -- ie, it must be compatable with range-based for loops.
 
    Here is an example implementation for a custom type:
    @code
    template<typename ElementType>
    struct MyContainerType
    {
        ElementType* begin();
        ElementType* end();
 
        void setSize (int);
    }
 
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
    @endcode
    @see TreeReflector, getInterfaceForContainer(), isContainer
 */
struct ContainerInterface
{
    virtual ~ContainerInterface() = default;

    virtual void resize (int newSize) = 0;
};


/**
    Template function that returns an instance of the correct ContainerInterface for the given container.
    To add support for a custom container type to TreeReflector::add():
    - Implement a subclass of ContainerInterface with the correct resizing logic for your cotainer;
    - Implement a specialization of this function that returns an instance of your custom interface;
    - Specialize the isContainer struct for your type and make the specialization inherit from std::true_type.
 
    In addition, your custom container type must have begin() and end() functions -- ie, it must be compatable with range-based for loops.
 
    Here is an example implementation for a custom type:
    @code
    template<typename ElementType>
    struct MyContainerType
    {
        ElementType* begin();
        ElementType* end();
 
        void setSize (int);
    }
 
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
    @endcode
    @see TreeReflector, ContainerInterface, isContainer
 */
template < typename ContainerType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (ContainerType&);


/**
    Template struct that determines if a given type is a container.
    To add support for a custom container type to TreeReflector::add():
    - Implement a subclass of ContainerInterface with the correct resizing logic for your cotainer;
    - Implement a specialization of this function that returns an instance of your custom interface;
    - Specialize the isContainer struct for your type and make the specialization inherit from std::true_type.
 
    In addition, your custom container type must have begin() and end() functions -- ie, it must be compatable with range-based for loops.
 
    Here is an example implementation for a custom type:
    @code
    template<typename ElementType>
    struct MyContainerType
    {
        ElementType* begin();
        ElementType* end();
 
        void setSize (int);
    }
 
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
    @endcode
    @see TreeReflector, ContainerInterface, getInterfaceForContainer()
 */
template < typename Type >
struct isContainer : std::false_type
{
};

}  // namespace lemons::serializing
