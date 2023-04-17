#include <QtWidgets/QFileDialog>
#include "settings_window.hpp"
#include "ui_settings_window.h"

//============================================================================================================
//============================================================================================================
SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
}

//============================================================================================================
//============================================================================================================
SettingsWindow::~SettingsWindow()
{
    delete ui;
}

//============================================================================================================
//Функция, при необходимости меняющая язык графического интерфейса.
//============================================================================================================
void SettingsWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//============================================================================================================
//Установка флага settingsAccepted в состояние True. Закрытие окна.
//============================================================================================================
void SettingsWindow::on_okButton_clicked()
{
    emit settingsUpdated();
    close();
}

//============================================================================================================
//Установка флага settingsAccepted в состояние False. Закрытие окна.
//============================================================================================================
void SettingsWindow::on_cancelButton_clicked()
{
    close();
}

//============================================================================================================
//Вызов диалогового окна для указания исполняемого файла Scripter'а.
//============================================================================================================
void SettingsWindow::on_scripterPathButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Укажите исполняемый файл Scripter'a"),
                                                    qApp->applicationDirPath(),
                                                    tr("Исполняемые файлы (*.exe)"));
    if(!filename.isEmpty())
        ui->scripterPathLineEdit->setText(filename);
}

//============================================================================================================
//Установка значений всех виджетов в соответствии с текущими настройками.
//============================================================================================================
void SettingsWindow::init(const Settings& settings)
{
    ui->useUniqFoldersCheckBox->setChecked(settings.useUniqFolders);

    ui->scripterPathLineEdit->setText(settings.pathToScripter);

    switch(settings.paintEngin)
    {
    case OpenGL:
        ui->paintEngineComboBox->setCurrentIndex(0);
        break;
    case Surfer:
        ui->paintEngineComboBox->setCurrentIndex(1);
        break;
    }

    switch(settings.colorScheme)
    {
    case BlueRed:
        ui->glColorSchemeComboBox->setCurrentIndex(0);
        break;
    case Rainbow:
        ui->glColorSchemeComboBox->setCurrentIndex(1);
        break;
    }

    switch(settings.surferVersion)
    {
    case Surfer8:
        ui->surferVersionComboBox->setCurrentIndex(0);
        break;
    case Surfer9:
        ui->surferVersionComboBox->setCurrentIndex(1);
        break;
    }
}

//============================================================================================================
//Получить новые значения настроек.
//============================================================================================================.
Settings SettingsWindow::getSettings()
{
    Settings settings;

    settings.useUniqFolders = ui->useUniqFoldersCheckBox->isChecked();

    settings.pathToScripter = ui->scripterPathLineEdit->text();

    if(ui->paintEngineComboBox->currentIndex() == 0)
        settings.paintEngin = OpenGL;
    else
        settings.paintEngin = Surfer;

    if(ui->glColorSchemeComboBox->currentIndex() == 0)
        settings.colorScheme = BlueRed;
    else
        settings.colorScheme = Rainbow;

    if(ui->surferVersionComboBox->currentIndex() == 0)
        settings.surferVersion = Surfer8;
    else
        settings.surferVersion = Surfer9;

    return settings;
}
