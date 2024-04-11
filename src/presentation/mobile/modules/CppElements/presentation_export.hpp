#pragma once
#include <QtCore/QtGlobal>

#if defined(PRESENTATION_LIBRARY)
    #define PRESENTATION_EXPORT Q_DECL_EXPORT
#else
    #define PRESENTATION_EXPORT Q_DECL_IMPORT
#endif