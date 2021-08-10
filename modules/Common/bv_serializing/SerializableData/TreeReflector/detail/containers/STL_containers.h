#pragma once

namespace bav::TreeReflectorHelpers
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
constexpr bool isContainer (std::array< ElementType, size >&)
{
    return true;
}


template < typename ElementType >
struct StdVectorInterface : ContainerInterface
{
    using VectorType = std::vector< ElementType >;

    StdVectorInterface (VectorType& vecToUse)
        : vector (vecToUse)
    {
    }

private:
    void resize (int newSize) final
    {
        vector.resize (static_cast< typename VectorType::size_type > (newSize));
    }

    VectorType& vector;
};

template < typename ElementType >
std::unique_ptr< ContainerInterface > getInterfaceForContainer (std::vector< ElementType >& container)
{
    return std::make_unique< StdVectorInterface< ElementType > > (container);
}

template < typename ElementType >
constexpr bool isContainer (std::vector< ElementType >&)
{
    return true;
}

}  // namespace bav::TreeReflectorHelpers
