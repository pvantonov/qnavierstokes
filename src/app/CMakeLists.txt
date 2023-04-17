add_executable(qnavierstokes
    about_window.cpp
    about_window.hpp
    about_window.ui
    help_window.hpp
    help_window.cpp
    ipainter.hpp
    main.cpp
    main_window.cpp
    main_window.hpp
    main_window.ui
    paint_aim.hpp
    settings.hpp
    settings_window.cpp
    settings_window.hpp
    settings_window.ui
)

target_link_libraries(qnavierstokes PRIVATE
    bottom_heating
    emf_painter
    opengl_painter
    side_heating
    color_scale
    Qt6::Widgets
    Qt6::WebEngineWidgets
    Qt6::Xml
)

set_target_properties(qnavierstokes PROPERTIES
    WIN32_EXECUTABLE ON
)

install(TARGETS qnavierstokes
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
