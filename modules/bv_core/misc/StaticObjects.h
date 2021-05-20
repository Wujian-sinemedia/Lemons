
#pragma once

namespace bav
{
template < typename T, typename... Args >
T& getStaticObject (Args&&... args)
{
    static std::unique_ptr< T > object;

    if (! object)
        object.reset (new T (std::forward< Args > (args)...));

    return *object;
}

}  // namespace bav
