#pragma once
#include <QtCore/QtGlobal>

#if defined(APPLICATION_LIBRARY)
    #  define APPLICATION_LIBRARY Q_DECL_EXPORT
#else
    #  define APPLICATION_LIBRARY Q_DECL_IMPORT
#endif
