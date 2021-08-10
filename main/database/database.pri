!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
    PRECOMPILED_HEADER += $$PWD/precompiled.h
} # !contains(DEFINES, _QX_NO_PRECOMPILED_HEADER)

DEFINES += _BUILDING_QX_API

HEADERS += \
    $$PWD/export.h \
    $$PWD/patient.h \
    $$PWD/precompiled.h \
    $$PWD/program.h \
    $$PWD/checkResult.h \
    $$PWD/checkResultVm.h \
    $$PWD/databaseSvc.h \
    $$PWD/patientVm.h

SOURCES += \
    $$PWD/patient.cpp \
    $$PWD/program.cpp \
    $$PWD/checkResult.cpp \
    $$PWD/checkResultVm.cpp \
    $$PWD/databaseSvc.cpp \
    $$PWD/patientVm.cpp

