!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
    PRECOMPILED_HEADER += $$PWD/precompiled.h
} # !contains(DEFINES, _QX_NO_PRECOMPILED_HEADER)

DEFINES += _BUILDING_QX_API

HEADERS += \
    $$PWD/paramsvm.h \
    $$PWD/programvm.h \
    $$PWD/checkResultVm.h \
    $$PWD/paramsvm.h \
    $$PWD/patientVm.h \
    $$PWD/programvm.h \
    $$PWD/patientListModelVm.h

SOURCES += \
    $$PWD/paramsvm.cpp \
    $$PWD/checkResultVm.cpp \
    $$PWD/paramsvm.cpp \
    $$PWD/patientVm.cpp \
    $$PWD/patientListModelVm.cpp

DISTFILES += \
    $$PWD/viewModel.pri

