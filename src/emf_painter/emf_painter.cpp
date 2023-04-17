#include <wtypes.h>
#include <windows.h>
#include <QtCore/QFile>
#include <QtGui/QPainter>
#include <QtGui/QPaintEngine>
#include <QtWidgets/QApplication>
#include "emf_painter.hpp"

//============================================================================================================
//============================================================================================================
EmfPainter::EmfPainter(QWidget *parent) : QWidget(parent)
{
    paintAim = NotDefine;
}

//============================================================================================================
//Установить цель для отрисовки (какую из найденных функций необходимо нарисовать).
//============================================================================================================
void EmfPainter::setPaintAim(PaintAim aim)
{
    paintAim = aim;
    update();
}

//============================================================================================================
//Нарисовать emf файл.
//============================================================================================================
void EmfPainter::paintEmfFile(const QString& filepath)
{
    QPainter painter(this);

    if(!QFile(filepath).exists())
        painter.drawText(rect(),Qt::AlignHCenter | Qt::AlignVCenter, tr("Файл изображения \"") + filepath
                                                                        + tr("\" не найден"));
    else
    {
//        //************************************************************************************************
//        //Переводим QString в wchar_t.
//        //************************************************************************************************
//        WCHAR wcFilename[1000];
//        QByteArray cFilename = QString(filepath).toAscii();
//        MultiByteToWideChar(CP_ACP, 0, cFilename.data(), strlen(cFilename.data())+1,
//                            wcFilename, sizeof(wcFilename)/sizeof(wcFilename[0]));

//        //************************************************************************************************
//        //************************************************************************************************
//        HDC hdc = painter.paintEngine()->getDC();
//        HENHMETAFILE hemf = GetEnhMetaFile(wcFilename);

//        ENHMETAHEADER header;
//        GetEnhMetaFileHeader(hemf,sizeof(ENHMETAHEADER),&header);

//        //************************************************************************************************
//        //Подгоняем размеры картинки и ее позицию так, чтобы она была видна вся и находилась по центру.
//        //************************************************************************************************
//        double aspectRatio = width();
//        aspectRatio /= qMax(header.rclBounds.right,header.rclBounds.bottom);

//        int left = (int)(width() - header.rclBounds.right*aspectRatio)/2;
//        int top = (int)(height() - header.rclBounds.bottom*aspectRatio)/2;
//        int right = left + (int)(header.rclBounds.right*aspectRatio);
//        int bottom = top + (int)(header.rclBounds.bottom*aspectRatio);

//        RECT rect = {13 + left, 33 + top, right, bottom};

//        //************************************************************************************************
//        //************************************************************************************************
//        PlayEnhMetaFile(hdc, hemf, &rect);
//        DeleteEnhMetaFile(hemf);
//        painter.paintEngine()->releaseDC(hdc);
    }
}

//============================================================================================================
//Функция отрисовки виджета.
//============================================================================================================
void EmfPainter::paintEvent(QPaintEvent* event)
{
    switch(paintAim)
    {
    case T:
        paintEmfFile(qApp->applicationDirPath() + "/result/T.emf");
        break;
    case Psi:
        paintEmfFile(qApp->applicationDirPath() + "/result/Psi.emf");
        break;
    case Omega:
        paintEmfFile(qApp->applicationDirPath() + "/result/Omega.emf");
        break;
    case Vx:
        paintEmfFile(qApp->applicationDirPath() + "/result/Vx.emf");
        break;
    case Vy:
        paintEmfFile(qApp->applicationDirPath() + "/result/Vy.emf");
        break;
    default:
        break;
    }
}
