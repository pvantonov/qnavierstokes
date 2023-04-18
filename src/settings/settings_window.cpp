#include <QtWidgets/QFileDialog>
#include "settings_window.hpp"
#include "ui_settings_window.h"
#include "settings_manager.hpp"

SettingsWindow::SettingsWindow(QWidget *parent) : QDialog(parent), _ui(new Ui::SettingsWindow)
{
    this->_ui->setupUi(this);
    this->_adaptSettings();
    this->connect(
        this->_ui->scripterPathButton, &QPushButton::clicked,
        this, &SettingsWindow::_selectScripterPath
    );
    this->connect(
        this->_ui->cancelButton, &QPushButton::clicked,
        this, &SettingsWindow::_rollbackSettings
    );
    this->connect(
        this->_ui->okButton, &QPushButton::clicked,
        this, &SettingsWindow::_commitCommitSettings
    );
}

SettingsWindow::~SettingsWindow()
{
    delete this->_ui;
}

void SettingsWindow::_adaptSettings()
{
    auto settings = SettingsManager::instance().settings();

    this->_ui->useUniqFoldersCheckBox->setChecked(settings.useUniqueFolders);
    this->_ui->scripterPathLineEdit->setText(settings.pathToScripter);

    switch(settings.paintEngine)
    {
    case PaintEngine::OpenGL:
        this->_ui->paintEngineComboBox->setCurrentIndex(0);
        break;
    case PaintEngine::Surfer:
        this->_ui->paintEngineComboBox->setCurrentIndex(1);
        break;
    }

    switch(settings.colorScheme)
    {
    case ColorScheme::BlueRed:
        this->_ui->glColorSchemeComboBox->setCurrentIndex(0);
        break;
    case ColorScheme::Rainbow:
        this->_ui->glColorSchemeComboBox->setCurrentIndex(1);
        break;
    }

    switch(settings.surferVersion)
    {
    case SurferVersion::Surfer8:
        this->_ui->surferVersionComboBox->setCurrentIndex(0);
        break;
    case SurferVersion::Surfer9:
        this->_ui->surferVersionComboBox->setCurrentIndex(1);
        break;
    }
}

void SettingsWindow::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        this->_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsWindow::_commitCommitSettings()
{
    Settings settings;

    settings.useUniqueFolders = this->_ui->useUniqFoldersCheckBox->isChecked();
    settings.pathToScripter = this->_ui->scripterPathLineEdit->text();

    if(this->_ui->paintEngineComboBox->currentIndex() == 0)
        settings.paintEngine = PaintEngine::OpenGL;
    else
        settings.paintEngine = PaintEngine::Surfer;

    if(this->_ui->glColorSchemeComboBox->currentIndex() == 0)
        settings.colorScheme = ColorScheme::BlueRed;
    else
        settings.colorScheme = ColorScheme::Rainbow;

    if(this->_ui->surferVersionComboBox->currentIndex() == 0)
        settings.surferVersion = SurferVersion::Surfer8;
    else
        settings.surferVersion = SurferVersion::Surfer9;

    SettingsManager::instance().setSettings(settings);

    this->close();
}

void SettingsWindow::_rollbackSettings()
{
    this->_adaptSettings();
    this->close();
}

void SettingsWindow::_selectScripterPath()
{
    QString filename = QFileDialog::getOpenFileName(
        this, tr("Укажите исполняемый файл Scripter'a"),
        qApp->applicationDirPath(),
        tr("Исполняемые файлы (*.exe)")
    );
    if(!filename.isEmpty())
        this->_ui->scripterPathLineEdit->setText(filename);
}
