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
    $$PWD/patientListModel.h \
    $$PWD/roles.h \
    $$PWD/patientVm.h

SOURCES += \
    $$PWD/checkResultVm.cpp \
    $$PWD/databaseSvc.cpp \
    $$PWD/patientListModel.cpp \
    $$PWD/patientVm.cpp \
    $$PWD/qx_register.cpp

