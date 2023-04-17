#include <QtWidgets/QToolBar>
#include <QtWidgets/QApplication>
#include "help_window.hpp"

//============================================================================================================
//============================================================================================================
HelpWindow::HelpWindow()
{
    view = new QWebEngineView(this);
    view->load(QUrl(qApp->applicationDirPath() + "/help/index.htm"));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustTitle()));
    connect(view, SIGNAL(titleChanged(const QString&)), SLOT(adjustTitle()));

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(view->pageAction(QWebEnginePage::Back));
    toolBar->addAction(view->pageAction(QWebEnginePage::Forward));

    setCentralWidget(view);
}

//============================================================================================================
//Смена заголовка окна в соответствии со сменой заголовка отображаемой веб-страницы.
//============================================================================================================
void HelpWindow::adjustTitle()
{
    setWindowTitle(view->title());
}

