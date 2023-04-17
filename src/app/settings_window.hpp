#pragma once
#include <QtWidgets/QDialog>
#include "settings.hpp"

namespace Ui
{
    class SettingsWindow;
}

//! Окно с настройками программы.
class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    //! \param[in] parent Указатель на родительский виджет.
    SettingsWindow(QWidget *parent = 0);

    ~SettingsWindow();

    /*!
     *  Установка значений всех виджетов в соответствии с текущими настройками.
     *  \param settings Текущие настройки программы.
     */
    void init(const Settings& settings);

    /*!
     *  Получить новые значения настроек.
     *  \return Новые значения настроек.
     */
    Settings getSettings();

signals:
    /*!
     *  \brief Сигнал о том, что с помощью диалогового окна были изменены настройки.
     *
     *  В данный момент сигнал вызывается  при нажатии кнопки "Применить настройки" независимо
     *  от того, были ли настройки реально изменены.
     */
    void settingsUpdated();

protected:
    //! Функция, при необходимости меняющая язык графического интерфейса.
    void changeEvent(QEvent *e);

private:
    Ui::SettingsWindow *ui; //!< Пользовательский интерфейс.

private slots:
    //! Вызов диалогового окна для указания исполняемого файла Scripter'а.
    void on_scripterPathButton_clicked();

    //! Сигнал об изменении настроек. Закрытие окна.
    void on_cancelButton_clicked();

    //! Закрытие окна.
    void on_okButton_clicked();
};
