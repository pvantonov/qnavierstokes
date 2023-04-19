add_executable(qnavierstokes
    about_window.cpp
    about_window.hpp
    about_window.ui
    ipainter.hpp
    main.cpp
    main_window.cpp
    main_window.hpp
    main_window.ui
    paint_aim.hpp
)

target_link_libraries(qnavierstokes PRIVATE
    bottom_heating
    opengl_painter
    side_heating
    color_scale
    reference
    settings
    Qt6::Widgets
)

set_target_properties(qnavierstokes PROPERTIES
    WIN32_EXECUTABLE ON
)
