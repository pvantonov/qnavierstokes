#ifndef GAUSS_GLOBAL_H
#define GAUSS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GAUSS_LIBRARY)
#  define GAUSSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define GAUSSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // GAUSS_GLOBAL_H
