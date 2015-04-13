#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWebKitWidgets/QWebView>

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
    QWebView *view;         //!< Виджет для отображения справки
};

#endif // HELPWINDOW_H
