!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
    PRECOMPILED_HEADER += $$PWD/precompiled.h
} # !contains(DEFINES, _QX_NO_PRECOMPILED_HEADER)

DEFINES += _BUILDING_QX_API

HEADERS += \
    $$PWD/export.h \
    $$PWD/patient.h \
    $$PWD/precompiled.h \
    $$PWD/check_result.h \
    $$PWD/program.h

SOURCES += \
    $$PWD/patient.cpp \
    $$PWD/check_result.cpp \
    $$PWD/program.cpp

