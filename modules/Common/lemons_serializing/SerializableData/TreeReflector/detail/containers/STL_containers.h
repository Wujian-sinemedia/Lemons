#pragma once

namespace lemons::serializing
{
template < typename ElementType, size_t size >
struct StdArrayInterface : LambdaContainerInterface< std::array< ElementType, size > >
{
    using Container = std::array< ElementType, size >;

    StdArrayInterface (Container& container)
        : LambdaContainerInterface< Container > (container, [] (Container&, int) {})
    {
    }
};

template < typename ElementType, size_t size >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (std::array< ElementType, size >& container)
{
    return std::make_unique< StdArrayInterface< ElementType, size > > (container);
}


template < typename ElementType, size_t size >
struct isContainer< std::array< ElementType, size > > : std::true_type
{
};


/*------------------------------------------------------------------------------------*/

template < class ContainerType >
struct STLContainerInterface : LambdaContainerInterface< ContainerType >
{
    STLContainerInterface (ContainerType& container)
        : LambdaContainerInterface< ContainerType > (container, [] (ContainerType& c, int newSize)
                                                     { c.resize (static_cast< typename ContainerType::size_type > (newSize)); })
    {
    }
};


template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (std::vector< ElementType >& container)
{
    return std::make_unique< STLContainerInterface< std::vector< ElementType > > > (container);
}

template < typename ElementType >
struct isContainer< std::vector< ElementType > > : std::true_type
{
};


template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (std::list< ElementType >& container)
{
    return std::make_unique< STLContainerInterface< std::list< ElementType > > > (container);
}

template < typename ElementType >
struct isContainer< std::list< ElementType > > : std::true_type
{
};

}  // namespace lemons::serializing
