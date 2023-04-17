add_library(emf_painter STATIC
    emf_painter.cpp
    emf_painter.hpp
)

target_link_libraries(emf_painter PRIVATE
    color_scale
    gdi32
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(
    emf_painter INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
