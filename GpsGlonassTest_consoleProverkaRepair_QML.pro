QT += qml quick quickwidgets serialport charts network sql printsupport quickcontrols2 axcontainer
CONFIG += c++11

TARGET = 'GPSGlonass test'

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
    algorithm.cpp \
    algorithmmodel.cpp \
    bdata.cpp \
    camber.cpp \
    cambermodel.cpp \
    clockcircle.cpp \
    finddevice.cpp \
    finddeviceitem.cpp \
    gsg.cpp \
    gsgmodel.cpp \
    n6700.cpp \
    n6700model.cpp \
    osuilograf.cpp \
    osuilografmodel.cpp \
    port.cpp \
    portmodel.cpp \
    proverkaitem.cpp \
    relizproverka.cpp \
    relizproverkatimer.cpp \
    report.cpp \
    tp8.cpp \
    tp8model.cpp \
    finddevicemodel.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



DISTFILES += \
    Header.qmlc \
    Page1Form.ui.qml \
    DevicePage.qml \
    Header.qml \
    Secondwindow.qml \
    Startwindow.qml

HEADERS += \
    algorithm.h \
    algorithmmodel.h \
    bdata.h \
    camber.h \
    cambermodel.h \
    clockcircle.h \
    finddevice.h \
    finddeviceitem.h \
    gsg.h \
    gsgmodel.h \
    n6700.h \
    n6700model.h \
    osuilograf.h \
    osuilografmodel.h \
    port.h \
    portmodel.h \
    proverkaitem.h \
    relizproverka.h \
    relizproverkatimer.h \
    report.h \
    tp8.h \
    tp8model.h \
    visa.h \
    visatype.h \
    finddevicemodel.h

# Загрузка иконки
RC_ICONS += logo_blue.ico

# Загрузка библиотеки (dll) для работы с устойствами по SCPI
# должна лежать в корне программы исходников для компиляции
LIBS += -L$$PWD/ -lMiVISA32
