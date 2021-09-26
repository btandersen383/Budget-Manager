

# Set list of UI files to compile
set (UIS
        mainwindow.ui
        ledgerlayout.ui
        categorylayout.ui
        filterledgerdialog.ui
        analysislayout.ui
        addledger.ui
        deleteledger.ui
        summarylayout.ui
        )
# Wrap UI files to generate source code
QT5_WRAP_UI (MOC_UI ${UIS})