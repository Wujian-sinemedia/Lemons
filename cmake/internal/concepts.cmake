if (NOT "${BV_CXX_VERSION}" STREQUAL "20")
    return()
endif()

include (CMakePushCheckState)
include (CheckIncludeFileCXX)
include (CheckCXXSourceCompiles)

cmake_push_check_state (RESET)

#

set (code [[
    template <typename T>
    concept Animal = requires(T a) {
        { a.make_sound() } -> void
    };

    template <Animal T>
    void make_sound(T animal) {
        animal.make_sound();
    }

    struct Cat {
        void make_sound() {
            /* Meow */
        }
    };

    int main() {
        Cat c;
        make_sound(c);
        return 0;
    }
]])

#

function (_bv_check_for_cxx_concepts)
    check_cxx_source_compiles ("${code}" HAVE_CXX_CONCEPTS)

    if (HAVE_CXX_CONCEPTS)
        message (STATUS "Success! Adding C++ concepts library.")
        add_library (CXX::Concepts INTERFACE IMPORTED)
    endif()

    unset (HAVE_CXX_CONCEPTS CACHE)
endfunction()

#

_bv_check_for_cxx_concepts()

if (NOT TARGET CXX::Concepts)

    set (CMAKE_REQUIRED_FLAGS -fconcepts)
    _bv_check_for_cxx_concepts()

    if (NOT TARGET CXX::Concepts)

        set (CMAKE_REQUIRED_DEFINITIONS "-Dconcept=concept\\ bool")
        _bv_check_for_cxx_concepts()

        if (NOT TARGET CXX::Concepts)
            message (WARNING "C++ concepts support not available for your compiler!")
            return()
        endif()

        target_compile_definitions (CXX::Concepts INTERFACE "concept=concept bool")

    endif()

    target_compile_options (CXX::Concepts INTERFACE -fconcepts)

endif()

if (TARGET CXX::Concepts)
    target_compile_definitions (CXX::Concepts INTERFACE CXX_CONCEPTS_AVAILABLE)
else()
    message (WARNING "C++ concepts support not available for your compiler!")
endif()

