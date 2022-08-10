
! contains( PERIMETER_MODULES, main ) {
  PERIMETER_MODULES *= main

  QT *= quick qml
  QT += sql
  HEADERS += \
    $$PWD/appctrl/perimeter_appctrl.hxx \
    \
    $$PWD/perimeter_main.hxx \

  SOURCES += \
    $$PWD/appctrl/perimeter_entry.cpp   \
    $$PWD/appctrl/perimeter_appctrl.cxx \
    \
    $$PWD/perimeter_main.cxx \

    RESOURCES += $$PWD/view/perimeter_main_view.qrc\
                 $$PWD/view/Controls/qml.qrc\
                 $$PWD/view/assets/assets.qrc

    # Additional import path used to resolve QML modules in Qt Creator's code model
    QML_IMPORT_PATH *= $$PWD/../../
    # Additional import path used to resolve QML modules just for Qt Quick Designer
    QML_DESIGNER_IMPORT_PATH =


   HEADERS += \
       $$PWD/Control/Calendar/perimeter_calendarsvc.hxx \
       $$PWD/Control/Calendar/perimeter_calendarvm.hxx \
       $$PWD/Control/Calendar/perimeter_ymdlistmodel.hxx

   SOURCES += \
       $$PWD/Control/Calendar/perimeter_calendarsvc.cxx \
       $$PWD/Control/Calendar/perimeter_calendarvm.cxx \
       $$PWD/Control/Calendar/perimeter_ymdlistmodel.cxx



    include( $$PWD/database/database.pri )
    include( $$PWD/model/model.pri )
    include( $$PWD/viewModel/viewModel.pri )
}

SOURCES += \
    $$PWD/appctrl/testclass.cpp \
    $$PWD/services/check_svc.cpp \
    $$PWD/services/analysis_svc.cpp \
    $$PWD/services/analysis_method_svc.cpp

HEADERS += \
    $$PWD/appctrl/testclass.h \
    $$PWD/services/check_svc.h \
    $$PWD/services/analysis_svc.h \
    $$PWD/services/analysis_method_svc.h

RESOURCES += \
    $$PWD/view/utils/utils.qrc

FORMS +=






