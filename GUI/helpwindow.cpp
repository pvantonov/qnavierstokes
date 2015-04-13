#include <QtGui/QToolBar>
#include <QtGui/QApplication>
#include "helpwindow.h"

//============================================================================================================
//============================================================================================================
HelpWindow::HelpWindow()
{
    view = new QWebView(this);
    view->load(QUrl(qApp->applicationDirPath() + "/help/index.htm"));
    connect(view, SIGNAL(loadFinished(bool)), SLOT(adjustTitle()));
    connect(view, SIGNAL(titleChanged(const QString&)), SLOT(adjustTitle()));

    QToolBar *toolBar = addToolBar(tr("Navigation"));
    toolBar->addAction(view->pageAction(QWebPage::Back));
    toolBar->addAction(view->pageAction(QWebPage::Forward));

    setCentralWidget(view);
}

//============================================================================================================
//Смена заголовка окна в соответствии со сменой заголовка отображаемой веб-страницы.
//============================================================================================================
void HelpWindow::adjustTitle()
{
    setWindowTitle(view->title());
}

