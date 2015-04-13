#ifndef LOS_GLOBAL_H
#define LOS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOS_LIBRARY)
#  define LOSSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LOSSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LOS_GLOBAL_H
