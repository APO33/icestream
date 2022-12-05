QT       += core gui core5compat network
#QT += core5compat

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
#QT_DEBUG_PLUGINS = 1
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    player.cpp

HEADERS += \
    mainwindow.h \
    player.h \
    ui_mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#DISTFILES += \
#    Icones_logiciel/MuteOff.ico \
#    Icones_logiciel/MuteOn.ico \
#    Icones_logiciel/Pause.ico \
#    Icones_logiciel/Play.ico \
#    Icones_logiciel/Solo.ico \
#    Icones_logiciel/SoloActiv.ico \
#    Icones_logiciel/Stop.ico \
#    Icones_logiciel/apo.bmp \
#    Icones_logiciel/apo.png \
#    Icones_logiciel/icestream.png \
#    Icones_logiciel/images.jpeg \
#    Icones_logiciel/leadImage.ico \
#    Icones_logiciel/leadImage.jpeg \
#    Icones_logiciel/led_green.ico \
#    Icones_logiciel/led_red.ico \
#    Icones_logiciel/new icone bleu.ico \
#    Icones_logiciel/new icone bleu.ico \
#    Icones_logiciel/new icone bleu.ico

RESOURCES += \
    icones.qrc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../../../../../lib/x86_64-linux-gnu/release/ -llo
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../../../../../lib/x86_64-linux-gnu/debug/ -llo
else:unix: LIBS += -L$$PWD/../../../../../../../../../lib/x86_64-linux-gnu/ -llo

INCLUDEPATH += $$PWD/../../../../../../../../../lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../../../../../lib/x86_64-linux-gnu

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/libraries/QOsc-master/build/src/release/ -lQOsc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/libraries/QOsc-master/build/src/debug/ -lQOsc
else:unix: LIBS += -L$$PWD/libraries/QOsc-master/build/src/ -lQOsc

INCLUDEPATH += $$PWD/libraries/QOsc-master/include
DEPENDPATH += $$PWD/libraries/QOsc-master/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libraries/QOsc-master/build/src/release/libQOsc.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libraries/QOsc-master/build/src/debug/libQOsc.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/libraries/QOsc-master/build/src/release/QOsc.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/libraries/QOsc-master/build/src/debug/QOsc.lib
else:unix: PRE_TARGETDEPS += $$PWD/libraries/QOsc-master/build/src/libQOsc.a
