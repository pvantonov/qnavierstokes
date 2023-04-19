#pragma once
#include <QtWidgets/QMainWindow>
#include "about_window.hpp"
#include "ui_main_window.h"
#include <opengl_painter.hpp>
#include <help_window.hpp>
#include <settings_window.hpp>
#include <side_heating_solver.hpp>
#include <bottom_heating_solver.hpp>

namespace Ui
{
    class MainWindow;
}

//! Главное окно программы.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //! \param[in] parent Указатель на родительский виджет.
    MainWindow(QWidget *parent = 0);

    ~MainWindow();

protected:
    //! Функция, при необходимости меняющая язык графического интерфейса.
    void changeEvent(QEvent *e);

private slots:
    void showSettingsWindow();
    void showHelpWindow();

    //! Сохранение отображенной функции в файл.
    void on_saveImageButton_clicked();

    //! Запуск решателя. Временная деактивация кнопки запуска решателя.
    void on_startButton_clicked();

    //! Прервать вычисление.
    void on_stopButton_clicked();

    //! Установка числа узлов для сетки в значения по умолчанию.
    void on_heightComboBox_currentIndexChanged(int index);

    //! Установка параметров решателя в значения умолчанию.
    void on_tBCComboBox_currentIndexChanged(int index);

    //! Активация кнопки запуска решателя. Обработка результата.
    void onSolverFinished();

    //! Отрисока поля температуры.
    void on_paintTButton_clicked();

    //! Отрисовка изолиний функции тока.
    void on_paintVxButton_clicked();

    //! Отрисовка изолиний вихря.
    void on_paintVyButton_clicked();

    //! Отрисовка изолиний x-компоненты скорости.
    void on_paintPsiButton_clicked();

    //! Отрисовка изолиний y-компоненты скорости.
    void on_paintOmegaButton_clicked();

    //! Вывод информации о завершении очередной итерации решателя.
    void onIterationFinished(int currentIteration, int maxNumOfIterations ,double residual);

    //! Вывод информации о том, что решатель не смог найти решение.
    void onMaxIterNumAttained(double residual);

    void on_actionPrGr_toggled(bool checked);

private:
    Ui::MainWindow *ui;                 //!< Пользовательский интерфейс.
    OpenGLPainter *glPainter;           //!< Виджет, рисующий результаты расчетов с помощью OpenGL.
    AboutWindow *aboutWindow;           //!< Окно с информацией о настройках.
    QScopedPointer<SettingsWindow> settingsWindow;
    QScopedPointer<HelpWindow> helpWindow;

    SideHeatingSolver solverSideHeating;
    BottomHeatingSolver solverBottomHeating;
};
