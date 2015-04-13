#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "settings.h"
#include "helpwindow.h"
#include "aboutwindow.h"
#include "ui_mainwindow.h"
#include "settingswindow.h"
#include "../EmfPainter/emfpainter.h"
#include "../OpenGLPainter/openglpainter.h"
#include "../SideHeating/sideheatingsolver.h"
#include "../BottomHeating/bottomheatingsolver.h"

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
    //! Вызов диалогового окна с настройками программы.
    void showSettingsWindow();

    //! Сохранение отображенной функции в файл.
    void on_saveImageButton_clicked();

    //! Получение и сохранение новых настроек программы.
    void getSettings();

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
    Settings settings;                  //!< Настройки программы.
    Ui::MainWindow *ui;                 //!< Пользовательский интерфейс.
    PaintEngin curEngine;               //!< Текущий графический движок программы.
    QString uniqFoldername;             //!< Имя папки, в которую будет скопирован результат.
    EmfPainter *emfPainter;             //!< Виджет, позволяющий рисовать emf файлы.
    OpenGLPainter *glPainter;           //!< Виджет, рисующий результаты расчетов с помощью OpenGL.
    AboutWindow *aboutWindow;           //!< Окно с информацией о настройках.
    HelpWindow *helpWindow;             //!< Окно для отображения справки.
    SettingsWindow *settingsWindow;     //!< Окно настроек.

    SideHeatingSolver solverSideHeating;
    BottomHeatingSolver solverBottomHeating;


    //! Загрузить настройки программы из файла.
    int loadSettings();

    //! Сохранить настройки программы в файл.
    int saveSettings();

    //! Сохранить настройки по умолчанию в файл.
    int saveDefaultSettings();

    //! Создать скрипт Surfer'а для обработки результатов.
    int createSurferScript();
};

#endif // MAINWINDOW_H
