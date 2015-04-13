#include <QtCore/QTextCodec>
#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForTr(QTextCodec::codecForName("Windows-1251"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("Windows-1251"));

    QApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(true);

    MainWindow window;
    window.show();

    return app.exec();
}
