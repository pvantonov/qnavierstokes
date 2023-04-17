#include <QtCore/QFile>
#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QTextStream>
#include <QtGui/QOpenGLFunctions>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMessageBox>
#include "opengl_painter.hpp"

//============================================================================================================
//============================================================================================================
OpenGLPainter::OpenGLPainter(Settings settings, QWidget *parent) : QOpenGLWidget(parent)
{
    paintAim = NotDefine;
    scheme = settings.colorScheme;
}

//============================================================================================================
//Настройка OpenGL (Установка флагов OpenGL и т.д.). Вызывается автоматически при
//============================================================================================================
void OpenGLPainter::initializeGL()
{
    QColor clearColor(Qt::white);
    glClearColor(clearColor.redF(), clearColor.greenF(), clearColor.blueF(), clearColor.alphaF());
}

//============================================================================================================
//Адоптация вывода OpenGL к размеру виджета. Вызывается автоматически при
//============================================================================================================
void OpenGLPainter::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
}

void OpenGLPainter::paintGlFile(const QString& filepath)
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    if(!QFile(filepath).exists())
    {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, 600, 0, 600, -1, 1);

        QColor color(Qt::black);
        glColor3f(color.redF(), color.greenF(), color.blueF());
        // renderText(5,280,tr("Файл изображения \"") + filepath + tr("\" не найден"));
    }
    else
    {
        int r,g,b;
        double x1, x2, x3;
        double y1, y2, y3;
        double xMax, yMax;
        double numOfTriangles;

        QFile file(filepath);
        QTextStream iostream(&file);
        file.open(QIODevice::ReadOnly);

        //************************************************************************************************
        //Устанавливаем масштаб.
        //************************************************************************************************
        iostream >> xMax >> yMax;
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0, qMax(xMax,yMax), 0, qMax(xMax,yMax), -1, 1);

        //************************************************************************************************
        //Русуем функцию.
        //************************************************************************************************
        iostream >> numOfTriangles;
        glBegin(GL_TRIANGLES);
        for(int i = 0; i < numOfTriangles; i++)
        {
            iostream >> x1 >> y1 >> r >> g >> b;
            glColor3f(r/255., g/255., b/255.);
            glVertex2f(x1,y1);

            iostream >> x2 >> y2 >> r >> g >> b;
            glColor3f(r/255., g/255., b/255.);
            glVertex2f(x2,y2);

            iostream >> x3 >> y3 >> r >> g >> b;
            glColor3f(r/255., g/255., b/255.);
            glVertex2f(x3,y3);
        }
        glEnd();

        //************************************************************************************************
        //Русуем изолинии.
        //************************************************************************************************
        glBegin(GL_LINES);
        QColor color(Qt::black);
        glColor3f(color.redF(), color.greenF(), color.blueF());
        while(!iostream.atEnd())
        {
            iostream >> x1 >> y1 >> x2 >> y2;
            glVertex2f(x1,y1);
            glVertex2f(x2,y2);
        }
        glEnd();

        //************************************************************************************************
        //Русуем контур.
        //************************************************************************************************
        glColor3f(color.redF(), color.greenF(), color.blueF());
        glLineWidth(2);
        glBegin(GL_LINE_LOOP);
        glVertex2f(0,0);
        glVertex2f(xMax,0);
        glVertex2f(xMax,yMax);
        glVertex2f(0,yMax);
        glEnd();
        glLineWidth(1);

        file.close();
    }
}

//============================================================================================================
//Отрисовка OpenGL. Вызывается автоматически при отрисовке виджета.
//============================================================================================================
void OpenGLPainter::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    switch(paintAim)
    {
    case T:
        paintGlFile(qApp->applicationDirPath() + "/result/T.gl");
        break;
    case Psi:
        paintGlFile(qApp->applicationDirPath() + "/result/Psi.gl");
        break;
    case Omega:
        paintGlFile(qApp->applicationDirPath() + "/result/Omega.gl");
        break;
    case Vx:
        paintGlFile(qApp->applicationDirPath() + "/result/Vx.gl");
        break;
    case Vy:
        paintGlFile(qApp->applicationDirPath() + "/result/Vy.gl");
        break;
    default:
        break;
    }

    glFinish();
}

//============================================================================================================
//Установить цель для отрисовки (какую из найденных функций необходимо нарисовать).
//============================================================================================================
void OpenGLPainter::setPaintAim(PaintAim aim)
{
    paintAim = aim;
    update();
}

//============================================================================================================
//Установить цветовую схему для цветовой шкалы.
//============================================================================================================
void OpenGLPainter::setColorScheme(ColorScheme scheme)
{
    if(this->scheme != scheme)
    {
        this->scheme = scheme;
        processData();
    }
}

//============================================================================================================
//Обработка результатов расчета для ускорения последующей отрисовки.
//============================================================================================================
void OpenGLPainter::processData()
{
    if(QFile(qApp->applicationDirPath() + "/result/T.dat").exists())
        processDataFile(qApp->applicationDirPath() + "/result/T.dat");
    if(QFile(qApp->applicationDirPath() + "/result/Psi.dat").exists())
        processDataFile(qApp->applicationDirPath() + "/result/Psi.dat");
    if(QFile(qApp->applicationDirPath() + "/result/Omega.dat").exists())
        processDataFile(qApp->applicationDirPath() + "/result/Omega.dat");
    if(QFile(qApp->applicationDirPath() + "/result/Vx.dat").exists())
        processDataFile(qApp->applicationDirPath() + "/result/Vx.dat");
    if(QFile(qApp->applicationDirPath() + "/result/Vy.dat").exists())
        processDataFile(qApp->applicationDirPath() + "/result/Vy.dat");
}

//============================================================================================================
//Обработка файла с результатами расчетов для дальнейшего ускорения отрисовки.
//============================================================================================================
void OpenGLPainter::processDataFile(const QString& filepath)
{
    QFile file(filepath);
    QTextStream iostream(&file);

    QList<double> f;
    QList< QVector<int> > nvtr;
    QList< QVector<double> > xy;

    //********************************************************************************************************
    //Считываем данные из файла
    //********************************************************************************************************
    file.open(QIODevice::ReadOnly);
    while(!iostream.atEnd())
    {
        double function;
        QVector<double> point(2);

        iostream >> point[0] >> point[1] >> function;
        f.append(function);
        xy.append(point);
    }
    file.close();

    //Последняя строчка в файлах пустая
    f.removeLast();
    xy.removeLast();

    //********************************************************************************************************
    //Разбиваем расчетную область на треугольники.
    //********************************************************************************************************

    //Есть уже готовая информация о треугольниках. Просто считываем ее.
    if(QFile(qApp->applicationDirPath() + "/result/nvtr.dat").exists())
    {
        file.setFileName(qApp->applicationDirPath() + "/result/nvtr.dat");
        file.open(QIODevice::ReadOnly);
        while(!iostream.atEnd())
        {
            QVector<int> triangle(3);

            iostream >> triangle[0] >> triangle[1] >> triangle[2];
            nvtr.append(triangle);
        }
        file.close();

        //Последняя строчка в файлах пустая
        nvtr.removeLast();
    }
    //Есть только информация о расположении узлов сетки. Разбиваем область на треугольники вручную.
    else if(QFile(qApp->applicationDirPath() + "/result/net.dat").exists())
    {
        int nx, ny;

        file.setFileName(qApp->applicationDirPath() + "/result/net.dat");
        file.open(QIODevice::ReadOnly);
        iostream >> nx >> ny;
        file.close();

        for(int i = 0; i < ny - 1; i++)
            for(int j = 0; j < nx - 1; j++)
            {
                QVector<int> triangle1(3);
                triangle1[0] = nx*i + j;
                triangle1[1] = nx*i + j + 1;
                triangle1[2] = nx*(i + 1) + j;

                QVector<int> triangle2(3);
                triangle2[0] = nx*(i + 1) + j;
                triangle2[1] = nx*(i + 1) + j + 1;
                triangle2[2] = nx*i + j + 1;

                nvtr.append(triangle1);
                nvtr.append(triangle2);
            }
    }
    //Информации о сетке нет. Дальнейшая работа невозможна.
    else
    {
        QMessageBox::critical(this,tr("Ошибка обработки данных"),tr("Невозможно найти файл\n\"") +
                              qApp->applicationDirPath() + "/result/nvtr.dat" + tr("\"\nили\n\"") +
                              qApp->applicationDirPath() + "/result/net.dat" +
                              tr("\"\nДальнейшая обработка данных невозможна!"));
        return;
    }

    //********************************************************************************************************
    //Находим минимальное и максимальное значение функции и максимальные значения координат.
    //Создаем цветовую шкалу.
    //********************************************************************************************************
    double minF(f[0]), maxF(f[0]);
    for(int i = 0; i < f.size(); i++)
    {
        if(f[i] < minF)
            minF = f[i];

        if(f[i] > maxF)
            maxF = f[i];
    }

    double maxX(0), maxY(0);
    for(int i = 0; i < xy.size(); i++)
    {
        if(xy[i][0] > maxX)
            maxX = xy[i][0];

        if(xy[i][1] > maxY)
            maxY = xy[i][1];
    }

    ColorScale colorScale;
    colorScale.initScale(minF,maxF,scheme);

    //********************************************************************************************************
    //Записываем файл с информацией о рисунке.
    //********************************************************************************************************
    //Сама функция
    QFile ofile(filepath.left(filepath.size() - 4) + ".gl");
    QTextStream ostream(&ofile);

    ofile.open(QIODevice::WriteOnly);
    ostream << maxX << " " << maxY << Qt::endl;
    ostream << nvtr.size() << Qt::endl;
    for(int i = 0; i < nvtr.size(); i++)
    {
        ostream << xy[nvtr[i][0]][0] << " " << xy[nvtr[i][0]][1] << " " << colorScale.getColor(f[nvtr[i][0]]).red() <<
            " " << colorScale.getColor(f[nvtr[i][0]]).green() << " " << colorScale.getColor(f[nvtr[i][0]]).blue() << Qt::endl;
        ostream << xy[nvtr[i][1]][0] << " " << xy[nvtr[i][1]][1] << " " << colorScale.getColor(f[nvtr[i][1]]).red() <<
            " " << colorScale.getColor(f[nvtr[i][1]]).green() << " " << colorScale.getColor(f[nvtr[i][1]]).blue() << Qt::endl;
        ostream << xy[nvtr[i][2]][0] << " " << xy[nvtr[i][2]][1] << " " << colorScale.getColor(f[nvtr[i][2]]).red() <<
            " " << colorScale.getColor(f[nvtr[i][2]]).green() << " " << colorScale.getColor(f[nvtr[i][2]]).blue() << Qt::endl;
    }

    //Изолинии
    double x[2],y[2];
    double isolines[15];
    for(int i = 0; i < 15; i++)
        isolines[i] = minF + (i + 1)*(maxF - minF)/16.;

    for(int i = 0; i < nvtr.size(); i++)
        for(int j = 0; j < 15; j++)
        {
            int l = 0;
            for(int k = 0; k < 3 && l < 2; k++)
            {
                if((f[nvtr[i][k%3]] >= isolines[j] && f[nvtr[i][(k + 1) %3]] < isolines[j])
                    || (f[nvtr[i][k%3]] < isolines[j] && f[nvtr[i][(k + 1) %3]] >= isolines[j]))
                    {
                        double a = (isolines[j] - f[nvtr[i][k%3]]) / (f[nvtr[i][(k + 1) %3]] - f[nvtr[i][k%3]]);
                        x[l] = xy[nvtr[i][k%3]][0] + a* (xy[nvtr[i][(k + 1) %3]][0] - xy[nvtr[i][k%3]][0]);
                        y[l] = xy[nvtr[i][k%3]][1] + a* (xy[nvtr[i][(k + 1) %3]][1] - xy[nvtr[i][k%3]][1]);
                        l++;
                    }
                if(f[nvtr[i][k%3]] == isolines[j] && f[nvtr[i][(k + 1) %3]] == isolines[j])
                {
                    x[0] = xy[nvtr[i][k%3]][0];
                    y[0] = xy[nvtr[i][k%3]][1];
                    x[1] = xy[nvtr[i][(k + 1) %3]][0];
                    y[1] = xy[nvtr[i][(k + 1) %3]][1];
                    l = 2;
                }
            }

            if (l == 2)
                ostream << x[0] << " " << y[0] << " " << x[1] << " " << y[1] << Qt::endl;
        }

    ofile.close();
}
