#pragma once
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWebEngineWidgets/QWebEngineView>

//!< Окно для просмотра справочной информации по программе.
class HelpWindow : public QMainWindow
{
    Q_OBJECT

public:
    HelpWindow();

protected slots:
    //! Смена заголовка окна в соответствии со сменой заголовка отображаемой веб-страницы.
    void adjustTitle();

private:
    QWebEngineView *view;         //!< Виджет для отображения справки
};
