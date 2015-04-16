#include <QtWidgets/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication::setOrganizationName("ami");
    QApplication::setApplicationName("qnavierstokes");
    QApplication::setApplicationVersion("1.5.0");
    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);

    MainWindow window;
    window.show();

    return app.exec();
}
