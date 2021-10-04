#pragma once

#include <forward_list>


namespace lemons::serializing
{
/** Container interface that provides support for serializing std::arrays with TreeReflector.
    std::array cannot actually be resized, so this interface does nothing when its resize() function is called.
 */
template < typename ElementType, size_t size >
struct StdArrayInterface : LambdaContainerInterface< std::array< ElementType, size > >
{
    using Container = std::array< ElementType, size >;

    StdArrayInterface (Container& container)
        : LambdaContainerInterface< Container > (container, [] (Container&, int) {})
    {
    }
};


/** Returns an instance of StdArrayInterface for the given std::array. */
template < typename ElementType, size_t size >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (std::array< ElementType, size >& container)
{
    return std::make_unique< StdArrayInterface< ElementType, size > > (container);
}


/** Specifies that any specialization of std::array is a serializable container. */
template < typename ElementType, size_t size >
struct isContainer< std::array< ElementType, size > > : std::true_type
{
};


/*------------------------------------------------------------------------------------*/


/** Container interface that provides support for serializing most STL containers with TreeReflector. */
template < class ContainerType >
struct STLContainerInterface : LambdaContainerInterface< ContainerType >
{
    STLContainerInterface (ContainerType& container)
        : LambdaContainerInterface< ContainerType > (container, [] (ContainerType& c, int newSize)
                                                     { c.resize (static_cast< typename ContainerType::size_type > (newSize)); })
    {
    }
};


/** Returns an instance of STLContainerInterface for the given std::vector. */
template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (std::vector< ElementType >& container)
{
    return std::make_unique< STLContainerInterface< std::vector< ElementType > > > (container);
}


/** Specifies that any specialization of std::vector is a serializable container. */
template < typename ElementType >
struct isContainer< std::vector< ElementType > > : std::true_type
{
};


/** Returns an instance of STLContainerInterface for the given std::list. */
template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (std::list< ElementType >& container)
{
    return std::make_unique< STLContainerInterface< std::list< ElementType > > > (container);
}


/** Specifies that any specialization of std::list is a serializable container. */
template < typename ElementType >
struct isContainer< std::list< ElementType > > : std::true_type
{
};


/** Returns an instance of STLContainerInterface for the given std::forward_list. */
template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (std::forward_list< ElementType >& container)
{
    return std::make_unique< STLContainerInterface< std::forward_list< ElementType > > > (container);
}


/** Specifies that any specialization of std::forward_list is a serializable container. */
template < typename ElementType >
struct isContainer< std::forward_list< ElementType > > : std::true_type
{
};


/** Returns an instance of STLContainerInterface for the given std::deque. */
template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (std::deque< ElementType >& container)
{
    return std::make_unique< STLContainerInterface< std::deque< ElementType > > > (container);
}


/** Specifies that any specialization of std::deque is a serializable container. */
template < typename ElementType >
struct isContainer< std::deque< ElementType > > : std::true_type
{
};

}  // namespace lemons::serializing
