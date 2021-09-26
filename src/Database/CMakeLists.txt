
set (LIBNAME dbhelp)

set (${LIBNAME}_SOURCES
        DbManagement.cpp
        DbLedger.cpp
        DbCategory.cpp
        SqlQueryModelFactory.cpp
        )

set (${LIBNAME}_HEADERS
        DbManagement.h
        DbLedger.h
        DbCategory.h
        SqlQueryModelFactory.h
        )

add_library(${LIBNAME} SHARED ${${LIBNAME}_SOURCES} ${${LIBNAME}_HEADERS})
