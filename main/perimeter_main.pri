
! contains( PERIMETER_MODULES, main ) {
  PERIMETER_MODULES *= main

  QT *= quick qml

  HEADERS += \
    $$PWD/appctrl/perimeter_appctrl.hxx \
    \
    $$PWD/perimeter_main.hxx \

  SOURCES += \
    $$PWD/appctrl/perimeter_entry.cpp   \
    $$PWD/appctrl/perimeter_appctrl.cxx \
    \
    $$PWD/perimeter_main.cxx \

  RESOURCES += $$PWD/view/perimeter_main_view.qrc
}

HEADERS += \
    $$PWD/customControl/Calendar/perimeter_calendarsvc.hxx \
    $$PWD/customControl/Calendar/perimeter_calendarvm.hxx \
    $$PWD/customControl/Calendar/perimeter_ymdlistmodel.hxx

SOURCES += \
    $$PWD/customControl/Calendar/perimeter_calendarsvc.cxx \
    $$PWD/customControl/Calendar/perimeter_calendarvm.cxx \
    $$PWD/customControl/Calendar/perimeter_ymdlistmodel.cxx

