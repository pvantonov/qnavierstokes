#pragma once
#include <QtCore/QString>

// Цветовая схема. Определяет какая цветовая гамма будет использоваться для отрисовки результата.
enum class ColorScheme
{
    BlueRed,
    Rainbow
};

// Движок отрисовки результатов.
enum class PaintEngine
{
    OpenGL,
    Surfer
};

enum class SurferVersion
{
    Surfer8,
    Surfer9
};

struct Settings
{
    bool useUniqueFolders;  // Запись каждого решения в отдельную папку
    QString pathToScripter;  // Путь к програме Scripter (поставляется вместе с программой Surfer)
    PaintEngine paintEngine;  // Используемый движок отрисовки результата
    ColorScheme colorScheme;  // Цветовая гамма, используемая для отрисовки результата
    SurferVersion surferVersion;  // Версия Surfer
    bool limitPrGr;  // Ограничивать значения чисел Pr и Gr
};
