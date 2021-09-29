#pragma once

namespace lemons::TreeReflectorHelpers
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


#define BV_TRF_DECLARE_STL_INTERFACE(STLClass, InterfaceClassName)    \
    BV_TRF_DECLARE_CONTAINER_INTERFACE (STLClass, InterfaceClassName, \
                                        container.resize (static_cast< typename STLClass< ElementType >::size_type > (newSize)))


BV_TRF_DECLARE_STL_INTERFACE (std::vector, StdVectorInterface)
BV_TRF_DECLARE_STL_INTERFACE (std::list, StdListInterface)

#undef BV_TRF_DECLARE_STL_INTERFACE

template < typename ElementType >
constexpr bool isContainer (std::vector< ElementType >&)
{
    return true;
}

}  // namespace bav::TreeReflectorHelpers
