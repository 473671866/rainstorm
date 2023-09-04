QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    application.cpp \
    button.cpp \
    dialog.cpp \
    dispaly.cpp \
    exit.cpp \
    main.cpp \
    profile.cpp \
    utils.cpp \
    virtualmachinecontrol.cpp

HEADERS += \
    application.h \
    button.h \
    dialog.h \
    dispaly.h \
    exit.h \
    profile.h \
    utils.h \
    virtualmachinecontrol.h \
    vix.h \
    vm_basic_types.h

FORMS += \
    application.ui \
    dialog.ui \
    virtualmachinecontrol.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

win32: LIBS += -L$$PWD/bin/vix/lib/ -lvix

INCLUDEPATH += $$PWD/bin/vix/include
DEPENDPATH += $$PWD/bin/vix/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/bin/vix/lib/vix.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/bin/vix/lib/libvix.a

win32: LIBS += -L$$PWD/bin/vix/lib/ -lvixd

INCLUDEPATH += $$PWD/''
DEPENDPATH += $$PWD/''

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/bin/vix/lib/vixd.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/bin/vix/lib/libvixd.a

win32: LIBS += -L$$PWD/bin/vnc/lib/ -lvncclient

INCLUDEPATH += $$PWD/bin/vnc/include
DEPENDPATH += $$PWD/bin/vnc/include

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/bin/vnc/lib/vncclient.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/bin/vnc/lib/libvncclient.a
