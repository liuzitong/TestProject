!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
    PRECOMPILED_HEADER += $$PWD/precompiled.h
} # !contains(DEFINES, _QX_NO_PRECOMPILED_HEADER)

DEFINES += _BUILDING_QX_API

HEADERS += \
    $$PWD/checkResultVm.h \
    $$PWD/patientVm.h \
    $$PWD/patientListModelVm.h \
    $$PWD/programListModelVm.h \
    $$PWD/programVm.h \
    $$PWD/paramsVm.h

SOURCES += \
    $$PWD/checkResultVm.cpp \
    $$PWD/patientVm.cpp \
    $$PWD/patientListModelVm.cpp \
    $$PWD/programListModelVm.cpp \
    $$PWD/paramsVm.cpp \
    $$PWD/programVm.cpp

DISTFILES += \
    $$PWD/viewModel.pri

