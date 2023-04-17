#pragma once
#include <QtCore/QString>
#include <color_scale.hpp>

//! Движок отрисовки результатов.
enum PaintEngin
{
    OpenGL,
    Surfer
};

//! Версия Surfer
enum SurferVersion
{
    Surfer8,
    Surfer9
};

//! Набор настроек программы.
struct Settings
{
    bool useUniqFolders;    //!< Запись каждого решения в отдельную папку.

    QString pathToScripter; //!< Путь к програме Scripter (поставляется вместе с программой Surfer).
    PaintEngin  paintEngin; //!< Используемый движок отрисовки результата.
    ColorScheme colorScheme;//!< Цветовая гамма, используемая для отрисовки результата.

    SurferVersion surferVersion;//!< Версия Surfer
};
