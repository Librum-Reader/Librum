#pragma once
#include <QtCore/QtGlobal>

#if defined(ADAPTERS_LIBRARY)
    #  define ADAPTERS_LIBRARY Q_DECL_EXPORT
#else
    #  define ADAPTERS_LIBRARY Q_DECL_IMPORT
#endif
