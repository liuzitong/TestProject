!contains(DEFINES, _QX_NO_PRECOMPILED_HEADER) {
    PRECOMPILED_HEADER += $$PWD/precompiled.h
} # !contains(DEFINES, _QX_NO_PRECOMPILED_HEADER)

DEFINES += _BUILDING_QX_API

HEADERS += \
    $$PWD/Params.h \
    $$PWD/programModel.h \
    $$PWD/Point.h \
    $$PWD/utility.h \
    $$PWD/checkResultModel.h

SOURCES +=

