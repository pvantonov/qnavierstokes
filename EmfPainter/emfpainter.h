#ifndef EMFPAINTER_H
#define EMFPAINTER_H

#include <QtGui/Qwidget>
#include "../GUI/paintaim.h"
#include "EmfPainter_global.h"

/*! \class Emfpainter
 *  \brief Виджет, рисующий emf файлы срезультатами вычислений.
 */
class EMFPAINTERSHARED_EXPORT EmfPainter : public QWidget
{
public:
    /*! \fn EmfPainter(QWidget *parent = 0)
     *  \param[in] parent Указатель на родительский виджет.
     */
    EmfPainter(QWidget *parent = 0);

    /*! \fn void setPaintAim(PaintAim aim)
     *  \brief Установить цель для отрисовки (какую из найденных функций необходимо нарисовать).
     *  \param aim Цель для отрисовки.
     */
    void setPaintAim(PaintAim aim);

protected:
    /*! \fn void paintEvent(QPaintEvent* event)
     *  \brief Функция отрисовки виджета.
     */
    void paintEvent(QPaintEvent* event);

private:
    /*! \fn void paintEmfFile(const QString& filepath)
     *  \brief Нарисовать emf файл.
     *  \param filepath Путь к файлу.
     */
    void paintEmfFile(const QString& filepath);


    /*! \var PaintAim paintAim;
     *  \brief Цель для отрисовки.
     */
    PaintAim paintAim;
};

#endif // EMFPAINTER_H
