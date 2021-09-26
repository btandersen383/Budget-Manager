

# Set list of QRC resource files
set (RESOURCES
        icons.qrc
        )

# Wrap QRC files to generate source code
QT5_ADD_RESOURCES (MOC_RESOURCES ${RESOURCES})