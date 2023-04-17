#pragma once
#include <QtOpenGLWidgets/QOpenGLWidget>
#include "../app/paint_aim.hpp"
#include "../app/settings.hpp"
#include <color_scale.hpp>

class OpenGLPainter : public QOpenGLWidget
{
    Q_OBJECT

public:
    /*!
     *  \param settings Набор настроек программы.
     *  \param[in] parent Указатель на родительский виджет.
     */
    OpenGLPainter(Settings settings, QWidget *parent = 0);

    //! Обработка результатов расчета для ускорения последующей отрисовки.
    void processData();

    /*!
     *  Установить цель для отрисовки (какую из найденных функций необходимо нарисовать).
     *  \param aim Цель для отрисовки.
     */
    void setPaintAim(PaintAim aim);

    /*!
     *  Установить цветовую схему для цветовой шкалы.
     *  \param scheme Цветовая схема.
     */
    void setColorScheme(ColorScheme scheme);

protected:
    //! Настройка OpenGL (Установка флагов OpenGL и т.д.).
    void initializeGL();

    /*!
     *  Адоптация вывода OpenGL к размеру виджета.
     *  \param w Новое значение ширины виджета.
     *  \param h Новое значение высоты виджета.
     */
    void resizeGL(int w, int h);

    //! Отрисовка OpenGL.
    void paintGL();

private:
    PaintAim paintAim;  //!< Цель для отрисовки.
    ColorScheme scheme; //!< Текущая цветовая схема.


    /*!
     *  Обработка файла с результатами расчетов для дальнейшего ускорения отрисовки.
     *  \param filepath Путь к файлу с результатом.
     */
    void processDataFile(const QString& filepath);

    /*!
     *  \brief Нарисовать gl файл. (Специально созданный для этой программы формат).
     *  \param filepath Путь к файлу.
     */
    void paintGlFile(const QString& filepath);
};
