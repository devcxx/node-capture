#include(../../common.pri)

QT       += widgets

TARGET = Screenshot
TEMPLATE = app

DEFINES += QT_STATICPLUGIN

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/main.cpp \
    src/amplifierwidget.cpp \
    src/buttonhandler.cpp \
    src/capturebutton.cpp \
    src/capturemodification.cpp \
    src/capturewidget.cpp \
    src/qcolorpicker.cpp \
    src/screenshot.cpp \
    src/confighandler.cpp \
    src/desktopfileparse.cpp \
    src/desktopinfo.cpp \
    src/screengrabber.cpp \
    src/applauncher.cpp \
    src/arrowtool.cpp \
    src/blurtool.cpp \
    src/capturetool.cpp \
    src/circletool.cpp \
    src/copytool.cpp \
    src/exittool.cpp \
    src/imagelabel.cpp \
    src/loadspinner.cpp \
    src/markertool.cpp \
    src/movetool.cpp \
    src/notificationwidget.cpp \
    src/penciltool.cpp \
    src/rectangletool.cpp \
    src/savetool.cpp \
    src/selectiontool.cpp \
    src/sizeindicatortool.cpp \
    src/toolfactory.cpp \
    src/undotool.cpp \
    src/activewindow.cpp \
    src/pinwidget.cpp

HEADERS += \
    include/screenshot_global.h \
    include/capturewidget.h \
    include/amplifierwidget.h \
    include/applauncher.h \
    include/arrowtool.h \
    include/blurtool.h \
    include/buttonhandler.h \
    include/capturebutton.h \
    include/capturemodification.h \
    include/capturetool.h \
    include/circletool.h \
    include/confighandler.h \
    include/copytool.h \
    include/desktopfileparse.h \
    include/desktopinfo.h \
    include/exittool.h \
    include/imagelabel.h \
    include/loadspinner.h \
    include/markertool.h \
    include/movetool.h \
    include/notificationwidget.h \
    include/penciltool.h \
    include/qcolorpicker.h \
    include/rectangletool.h \
    include/savetool.h \
    include/screengrabber.h \
    include/screenshot.h \
    include/screenshot_global.h \
    include/selectiontool.h \
    include/sizeindicatortool.h \
    include/toolfactory.h \
    include/undotool.h \
    include/activewindow.h \
    include/pinwidget.h

win32 {
    LIBS += -lUser32
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}

macos {
    ICON = $$PWD/res/Screenshot.icns
    LIBS += \
         -framework Cocoa

    HEADERS += \
         include/authscreencapture.h

    OBJECTIVE_SOURCES += \
         src/authscreencapture.mm
}

DESTDIR = $$PWD/bin

mac {
    QMAKE_POST_LINK += $$quote($$[QT_INSTALL_BINS]/macdeployqt \"$${DESTDIR}/$${TARGET}.app\" -libpath=\"$$DESTDIR\"$$escape_expand(\\n\\t))
}

INCLUDEPATH += $$PWD/include

TRANSLATIONS += $$PWD/res/zh_CN.ts

FORMS += \
    src/amplifierwidget.ui \
    src/qcolorpicker.ui \
    src/amplifierwidget.ui \
    src/qcolorpicker.ui \
    src/amplifierwidget.ui \
    src/qcolorpicker.ui

RESOURCES += \
    res/screenshot.qrc



#MOC_DIR = $$PWD/src
#RCC_DIR = $$PWD/src
#UI_DIR = $$PWD/src
