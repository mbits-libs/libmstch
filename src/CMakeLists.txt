set(mstch_INCLUDE_DIR
    ${PROJECT_SOURCE_DIR}/include CACHE STRING "mstch include directory")

set(SRC
    state/in_section.cpp
    state/outside_section.cpp
    state/render_state.hpp
    visitor/get_token.hpp
    visitor/has_token.hpp
    visitor/is_node_empty.hpp
    visitor/render_node.hpp
    visitor/render_section.hpp
    mstch.cpp
    render_context.cpp
    template_type.cpp
    token.cpp
    utils.cpp)

add_library(mstch STATIC ${SRC})
target_include_directories(mstch PUBLIC
    $<BUILD_INTERFACE:${mstch_INCLUDE_DIR}>
    $<INSTALL_INTERFACE:include>)

set_property(TARGET mstch PROPERTY VERSION ${mstch_VERSION})

if (WITH_INSTALLER)
    install(TARGETS mstch EXPORT mbits)
    install(EXPORT mbits NAMESPACE mbits:: DESTINATION lib/cmake)

    install(
        FILES "${PROJECT_SOURCE_DIR}/include/mstch/mstch.hpp"
        DESTINATION include/mstch
        COMPONENT Devel)
endif(WITH_INSTALLER)