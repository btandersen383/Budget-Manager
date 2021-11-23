cmake_minimum_required (VERSION 3.20)
project (BudgetMan)

# Set general cmake settings
set (CMAKE_CXX_STANDARD 14)
set (CMAKE_AUTOMOC ON)
set (CMAKE_AUTORCC ON)
set (CMAKE_AUTOUIC ON)

# Collect all required Qt5 packages locations
find_package (Qt5 COMPONENTS Core Gui Widgets Charts Sql
        REQUIRED)

# Set include paths for header includes
include_directories (${PROJECT_SOURCE_DIR})
include_directories (${PROJECT_BINARY_DIR})

# Set search path to find UI files
set (CMAKE_AUTOUIC_SEARCH_PATHS ui)

# Set list of source files to compile
set (SOURCES main.cpp src/MainWindow.cpp
        src/Database/DbManagement.cpp
        src/Database/DbLedger.cpp
        src/Database/DbCategory.cpp
        src/Database/SqlQueryModelFactory.cpp
        src/Widgets/TransactionsDelegate.cpp
        src/Widgets/DollarSpinBox.cpp
        src/Layouts/CategoryLayout.cpp
        src/Layouts/LedgerLayout.cpp
        src/Layouts/FilterLedgerDialog.cpp
        src/Layouts/ChartsAnalysisLayout.cpp
        src/Layouts/AddLedgerDialog.cpp
        src/Layouts/DeleteLedgerDialog.cpp
        src/Layouts/SummaryWidget.cpp
        )

# Set list of header files to wrap with Qt
set(HEADERS src/MainWindow.h
        src/Database/DbManagement.h
        src/Database/DbLedger.h
        src/Database/DbCategory.h
        src/Database/SqlQueryModelFactory.h
        src/Layouts/LedgerLayout.h
        src/Layouts/CategoryLayout.h
        src/Layouts/FilterLedgerDialog.h
        src/Layouts/ChartsAnalysisLayout.h
        src/Layouts/AddLedgerDialog.h
        src/Layouts/DeleteLedgerDialog.h
        src/Layouts/SummaryWidget.h
        src/Widgets/TransactionsDelegate.h
        src/Widgets/DollarSpinBox.h
        )
# Wrap header files with Qt
QT5_WRAP_CPP (MOC_HEADERS ${HEADERS})

# Set list of UI files to compile, sets ${MOC_UI}
add_subdirectory(ui)

# Set list of QRC resource files
set (RESOURCES resources/icons.qrc)
# Wrap QRC files to generate source code
QT5_ADD_RESOURCES (MOC_RESOURCES ${RESOURCES})

# Add the program executable
add_executable (Test ${SOURCES} ${MOC_HEADERS} ${MOC_UI} ${MOC_RESOURCES})

# Link the executable
target_link_libraries (Test Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Sql Qt5::Charts)
