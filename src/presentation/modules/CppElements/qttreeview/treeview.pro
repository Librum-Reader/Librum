!versionAtLeast(QT_VERSION, 5.15.0):error("This project requires Qt-5.15 or newer to build.")
requires(qtHaveModule(quick))
requires(qtHaveModule(quickcontrols2))

load(qt_parts)

