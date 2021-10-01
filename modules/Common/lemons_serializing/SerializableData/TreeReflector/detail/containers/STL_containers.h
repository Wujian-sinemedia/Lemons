#pragma once

namespace lemons::serializing::TreeReflectorHelpers
{
template < typename ElementType, size_t size >
struct StdArrayInterface : ContainerInterface
{
    using ArrayType = std::array< ElementType, size >;

    StdArrayInterface (ArrayType&);

private:
    void resize (int) final { }
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

template < typename ElementType >
struct StdVectorInterface : ContainerInterface
{
    using Type = std::vector< ElementType >;

    StdVectorInterface (Type& containerToUse)
        : container (containerToUse)
    {
    }

private:
    void resize (int newSize) final
    {
        container.resize (static_cast< typename Type::size_type > (newSize));
    }

    Type& container;
};

template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (std::vector< ElementType >& container)
{
    return std::make_unique< StdVectorInterface< ElementType > > (container);
}

template < typename ElementType >
struct isContainer< std::vector< ElementType > > : std::true_type
{
};

/*------------------------------------------------------------------------------------*/

template < typename ElementType >
struct StdListInterface : ContainerInterface
{
    using Type = std::list< ElementType >;

    StdListInterface (Type& containerToUse)
        : container (containerToUse)
    {
    }

private:
    void resize (int newSize) final
    {
        container.resize (static_cast< typename Type::size_type > (newSize));
    }

    Type& container;
};

template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (std::list< ElementType >& container)
{
    return std::make_unique< StdListInterface< ElementType > > (container);
}

template < typename ElementType >
struct isContainer< std::list< ElementType > > : std::true_type
{
};

}  // namespace lemons::serializing::TreeReflectorHelpers
