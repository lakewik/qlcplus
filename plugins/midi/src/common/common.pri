INCLUDEPATH += ../../../interfaces
INCLUDEPATH += ../../../../engine/src
INCLUDEPATH += ../common
DEPENDPATH  += ../common

CONFIG  += qt
QT      += core widgets

HEADERS     += ../../../interfaces/qlcioplugin.h
HEADERS     += ../../../../engine/src/qlcfile.h
HEADERS += ../common/mididevice.h \
           ../common/midiinputdevice.h \
           ../common/midioutputdevice.h \
           ../common/midiplugin.h \
           ../common/midiprotocol.h \
           ../common/miditemplate.h \
           ../common/midienumerator.h \
           ../common/configuremidiplugin.h \
           ../common/mtctimecode.h

SOURCES += ../../../interfaces/qlcioplugin.cpp
SOURCES += ../common/mididevice.cpp \
           ../common/midiinputdevice.cpp \
           ../common/midioutputdevice.cpp \
           ../common/midiplugin.cpp \
           ../common/midiprotocol.cpp \
           ../common/miditemplate.cpp \
           ../common/miditemplate.cpp \
           ../common/configuremidiplugin.cpp \
           ../common/mtctimecode.cpp

SOURCES += ../../../../engine/src/qlcfile.cpp

FORMS   += ../common/configuremidiplugin.ui

# Force MOC processing for MTCTimeCode
MOC_HEADERS += ../common/mtctimecode.h

# This must be after "TARGET = " and before target installation so that
# install_name_tool can be run before target installation
macx:include(../../../../platforms/macos/nametool.pri)

target.path = $$INSTALLROOT/$$PLUGINDIR
INSTALLS   += target

