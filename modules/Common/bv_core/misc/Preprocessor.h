#pragma once


#define BV_DECLARE_RECURSIVE_VARIADIC_FUNCTION(funcName, Type) \
    void funcName (Type);                                      \
    template < typename... Args >                              \
    void funcName (Type first, Args&&... rest)                 \
    {                                                          \
        funcName (first);                                      \
        funcName (std::forward< Args > (rest)...);             \
    }


#define BV_MUST_INHERIT_FROM(classToTest, requiredBaseClass) \
    std::enable_if_t< std::is_base_of< requiredBaseClass, classToTest >::value >* = nullptr


#define BV_TYPED_ENUM(Name, Type, ...)          \
    struct Name                                 \
    {                                           \
        enum : Type                             \
        {                                       \
            __VA_ARGS__                         \
        };                                      \
        static inline const size_t count = [] { \
            static Type __VA_ARGS__;            \
            return std::size ({__VA_ARGS__});   \
        }();                                    \
        using underlying_type = Type;           \
    };

#define BV_ENUM(Name, ...) BV_TYPED_ENUM (Name, int, __VA_ARGS__)
