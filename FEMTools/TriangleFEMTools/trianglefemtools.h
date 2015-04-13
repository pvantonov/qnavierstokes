#ifndef TRIANGLEFEMTOOLS_H
#define TRIANGLEFEMTOOLS_H

#include "trianglefemtools_global.h"

namespace trianglefemtools
{
    /*! \fn void formMatrixProfile(int numOfTriangles, int numOfPoints,	int **nvtr, int **ig, int **jg)
     *
     * \brief Формирование профиля матрицы конечноэлементной СЛАУ.
     *
     * \param numOfTriangles    Число треугольников в конечноэлементной сетке.
     * \param numOfPoints       Число узлов в конечноэлементной сетке.
     * \param [in]  nvtr        Информация о структуре конечноэлементной сетки.
     * \param [out] ig          Информация о количестве ненулевых элементов в каждой из строк матрицы.
     * \param [out] jg          Номера столбцов ненулевых элементов матрицы.
     */
    TRIANGLEFEMTOOLSSHARED_EXPORT
            void formMatrixProfile(int numOfTriangles, int numOfPoints, int **nvtr, int **ig, int **jg);
}

#endif // TRIANGLEFEMTOOLS_H
