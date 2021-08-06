QT -= gui
#QT += sql
CONFIG += console thread
CONFIG += c++11  qml_debug
CONFIG -= app_bundle
DEFINES += QT_DEPRECATED_WARNINGS QT_MESSAGELOGCONTEXT _QX_NO_PRECOMPILED_HEADER

INCLUDEPATH *= $$PWD/../../../
# ///////////////////////////////////////////////////////////////////////////
# sub function module
# ///////////////////////////////////////////////////////////////////////////
DEFINES *= PERIMETER_CFG_STATIC
include( $$PWD/../../base/perimeter_base.pri )
include( $$PWD/../../main/perimeter_main.pri )
include( $$PWD/../../third-part/qxpackic_all.pri )

# ///////////////////////////////////////////////////////////////////////////
# put the final app into /tmp for debug
# ///////////////////////////////////////////////////////////////////////////
APP_INSTALL_PATH=/tmp
target.path = $${APP_INSTALL_PATH}
INSTALLS += target


# QxOrm 库相关配置
INCLUDEPATH += $$PWD/../../../perimeter/third-part/QxOrm/include
LIBS += -L$$PWD/../../../perimeter/third-part/QxOrm/lib

# 设置生成的目标名称、添加依赖库
CONFIG(debug, debug|release) {
    LIBS += -l"QxOrmd"
} else {
    LIBS += -l"QxOrm"
}

DEFINES += _BUILDING_QX_API

