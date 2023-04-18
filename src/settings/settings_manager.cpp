#include <QtCore/QSettings>
#include <QtCore/QDir>
#include <QtWidgets/QApplication>
#include "settings_manager.hpp"
#include "settings.hpp"

SettingsManager::SettingsManager(QObject *parent) : QObject{parent}
{
    this->_loadSettings();
}

QString SettingsManager::_settingsPath()
{
    return QDir::cleanPath(qApp->applicationDirPath() + QDir::separator() + "qnavierstokes.ini");
}

void SettingsManager::_loadSettings()
{
    QSettings settings(SettingsManager::_settingsPath(), QSettings::IniFormat);
    this->_settings.useUniqueFolders = settings.value("common/use_unique_folders", QVariant(false)).toBool();
    this->_settings.pathToScripter = settings.value("painting/scripter_path").toString();
    this->_settings.limitPrGr = settings.value("common/limit_pr_gr", QVariant(true)).toBool();

    if (settings.contains("painting/engine")) {
        this->_settings.paintEngine = static_cast<PaintEngine>(settings.value("painting/engine").toInt());
    } else {
        this->_settings.paintEngine = PaintEngine::OpenGL;
    }

    if (settings.contains("painting/color_scheme")) {
        this->_settings.colorScheme = static_cast<ColorScheme>(settings.value("painting/color_scheme").toInt());
    } else {
        this->_settings.colorScheme = ColorScheme::BlueRed;
    }

    if (settings.contains("painting/surfer_version")) {
        this->_settings.surferVersion = static_cast<SurferVersion>(settings.value("painting/surfer_version").toInt());
    } else {
        this->_settings.surferVersion = SurferVersion::Surfer9;
    }
}

void SettingsManager::_saveSettings() const
{
    QSettings settings(SettingsManager::_settingsPath(), QSettings::IniFormat);
    settings.setValue("common/use_unique_folders", QVariant(this->_settings.useUniqueFolders));
    settings.setValue("painting/scripter_path", QVariant(this->_settings.pathToScripter));
    settings.setValue("painting/engine", QVariant(static_cast<int>(this->_settings.paintEngine)));
    settings.setValue("painting/color_scheme", QVariant(static_cast<int>(this->_settings.colorScheme)));
    settings.setValue("painting/surfer_version", QVariant(static_cast<int>(this->_settings.surferVersion)));
    settings.setValue("common/limit_pr_gr", QVariant(this->_settings.limitPrGr));
}